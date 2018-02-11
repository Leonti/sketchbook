//#include <Wire.h>
#include <Adafruit_TiCoServo.h>
#include <Adafruit_INA219.h>

#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Adafruit_INA219 ina219;
Adafruit_TiCoServo myservo;
int pos = 0;

bool isTurningOff = false;
long previousMillis = 0;
long tempRequestedMillis = 0;
bool tempRequested = false;
long toTurnOffMillis = 0;
bool powerButtonPressed = false;

const int POWER_BUTTON = 7;
const int POWER_OFF_PIN = 8; 
const int SERVO_PIN = 9;

void setup(void) 
{
  Serial.begin(115200);
  myservo.attach(SERVO_PIN);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }

  sensors.begin();
  sensors.setWaitForConversion(false);

  uint32_t currentFrequency;
    
  Serial.println("Hello!");

  pinMode(POWER_BUTTON, INPUT);
  pinMode(POWER_OFF_PIN, OUTPUT);

  digitalWrite(POWER_OFF_PIN, LOW);
  
  ina219.begin();

  Serial.println("Measuring voltage and current with INA219 ...");
}

void loop(void) 
{

  unsigned long currentMillis = millis();

  if (Serial.available() > 0) {
    processMessage();
  } else if (currentMillis - previousMillis > 2000) {
    previousMillis = currentMillis;
    
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
  
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
//    power = busvoltage * (current_mA/1000);


    // 1.2 x (ncellx - 1) = 6V = cut off voltage
    Serial.print("B:");
    Serial.print(loadvoltage); Serial.print(",");
    Serial.println(current_mA);

    if (!tempRequested) {
      sensors.requestTemperatures();
      tempRequestedMillis = currentMillis;
      tempRequested = true;   
    }
     
  } else if (tempRequested && currentMillis - tempRequestedMillis > 2000) {
    Serial.print("T:"); 
    Serial.print(sensors.getTempCByIndex(0)); Serial.print(",");
    Serial.println(sensors.getTempCByIndex(1));
    tempRequested = false;
  }  
  
  if (digitalRead(POWER_BUTTON) == HIGH) {

    if (powerButtonPressed == false) {
      Serial.println("BTN:1");
      powerButtonPressed = true;
    }
  } else {
    powerButtonPressed = false;
  }
  
  if (isTurningOff && millis() > toTurnOffMillis) {
    Serial.println("Turning off");
    turnOff();
  }
}

void turnOff() {
  digitalWrite(POWER_OFF_PIN, HIGH);
  isTurningOff = false;
  delay(500);
  digitalWrite(POWER_OFF_PIN, LOW);
}

void processMessage() {
  while (Serial.available() > 0) {

    char c = Serial.peek();
    // ignore newlines
    if (c == 'S') {
      Serial.println("OK");
      Serial.read();
      int pos = Serial.parseInt();
      myservo.write(pos);
    } else if (c == 'O') {
      Serial.println("OK");
      int offDelaySeconds = Serial.parseInt();
      toTurnOffMillis = millis() + offDelaySeconds * 1000;
      isTurningOff = true;
    } else {
      Serial.read(); 
    }
  }
}
