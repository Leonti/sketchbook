//#include <Wire.h>
#include <Adafruit_TiCoServo.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

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
unsigned long toTurnOffMillis = 0;
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

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  sensors.begin();
  sensors.setWaitForConversion(false);

  uint32_t currentFrequency;

  pinMode(POWER_BUTTON, INPUT);
  pinMode(POWER_OFF_PIN, OUTPUT);

  digitalWrite(POWER_OFF_PIN, LOW);
  
  ina219.begin();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.println(F("Rover v0.1"));
  display.display();
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
    Serial.print(F("B:"));
    Serial.print(loadvoltage); Serial.print(F(","));
    Serial.println(current_mA);

    display.setCursor(0,10);
    display.print(loadvoltage);
    display.print(F("V"));
    display.setCursor(0,20);
    display.print(current_mA);
    display.print(F("mA"));
    display.display();

    if (!tempRequested) {
      sensors.requestTemperatures();
      tempRequestedMillis = currentMillis;
      tempRequested = true;   
    }
     
  } else if (tempRequested && currentMillis - tempRequestedMillis > 2000) {
    Serial.print(F("T:")); 
    float roomTemp = sensors.getTempCByIndex(0);
    float batteryTemp = sensors.getTempCByIndex(1);
    Serial.print(roomTemp); Serial.print(",");
    Serial.println(batteryTemp);

    display.setCursor(0,30);
    display.print(F("R: "));
    display.print(roomTemp);
    display.print(F("C "));
    display.print(F("B: "));
    display.print(batteryTemp);
    display.print(F("C"));
    display.display();
    
    tempRequested = false;
  }  
  
  if (digitalRead(POWER_BUTTON) == HIGH) {

    if (powerButtonPressed == false) {
      Serial.println(F("BTN:1"));
      powerButtonPressed = true;
    }
  } else {
    powerButtonPressed = false;
  }
  
  if (isTurningOff && millis() > toTurnOffMillis) {    
    turnOff();
  }
}

void turnOff() {
  display.setCursor(0,40);
  display.println(F("Shutting down..."));
  display.display();
    
  digitalWrite(POWER_OFF_PIN, HIGH);
  isTurningOff = false;
  delay(500);
  digitalWrite(POWER_OFF_PIN, LOW);
}

void printShutdownConfirmation() {
  display.setCursor(0,40);
  display.println(F("Shutting requested!"));
  display.display();
}

void processMessage() {
  while (Serial.available() > 0) {

    char c = Serial.peek();
    // ignore newlines
    if (c == 'S') {
      Serial.println(F("OK"));
      Serial.read();
      int pos = Serial.parseInt();
      myservo.write(pos);
    } else if (c == 'O') {
      Serial.println(F("OK"));
      printShutdownConfirmation();
      Serial.read();
      long offDelaySeconds = Serial.parseInt();
      Serial.println(offDelaySeconds);
      toTurnOffMillis = millis() + (offDelaySeconds * 1000);
      isTurningOff = true;
    } else {
      Serial.read(); 
    }
  }
}
