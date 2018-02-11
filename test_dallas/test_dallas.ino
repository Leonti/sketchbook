#include <DallasTemperature.h>
#include <Servo.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

Servo myservo;

long previousMillis = 0;

long tempRequestedMillis = 0;

bool tempRequested = false;

void setup(void)
{
  // start serial port
  Serial.begin(115200);
  Serial.println("Dallas Temperature IC Control Library Demo");

  myservo.attach(9);

  // Start up the library
  sensors.begin();
  sensors.setWaitForConversion(false);
}

void loop(void)
{ 

  unsigned long currentMillis = millis();
  if (!tempRequested && currentMillis - previousMillis > 2000) {
    previousMillis = currentMillis;

    sensors.requestTemperatures(); // Send the command to get temperatures

    tempRequestedMillis = currentMillis;
    tempRequested = true;
  } else if (tempRequested && currentMillis - tempRequestedMillis > 2000) {
    Serial.println(sensors.getTempCByIndex(0));
    tempRequested = false;
  }
}
