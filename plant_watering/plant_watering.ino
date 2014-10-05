#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include <dht.h>
dht DHT;

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 12, 0, 10);
int sensor = 2;
int DHT11_PIN = 5;

void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);

  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(0,20);
  display.println("Guten Tag!");
  
  display.display();  
  
  delay(2000);
}

void loop() {

  refreshData();
 
  
  Serial.println(isDry());
  delay(2000);
}

void refreshData() {
  
  int chk = DHT.read11(DHT11_PIN);
  
  char temperature[3];
  dtostrf(DHT.temperature, 2, 0, temperature);
  Serial.println(temperature);

  char humidity[3];
  dtostrf(DHT.humidity, 2, 0, humidity);
  Serial.println(humidity);  
  
  display.clearDisplay();
  display.setCursor(0,10);
  display.print("Temp: ");
  display.print(temperature);
  
  display.setCursor(0,30);
  display.print("Humidity: ");
  display.print(humidity);   
                   
  display.display();
}

boolean isDry() {
  return digitalRead(sensor) == 1 ? false : true;
}
