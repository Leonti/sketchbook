#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include <dht.h>
dht DHT;

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 12, 0, 10);
int SENSOR = 4;
int PUMP = 3;
int DHT11_PIN = 5;

int PUMPING_TIME_LIMIT = 30;

boolean isPumping = false;
int pumpingTime = 0;

int wateredCount = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR, INPUT);
  digitalWrite(SENSOR, LOW);
  pinMode(PUMP, OUTPUT);

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
 
  if (isDry()) {
    if (pumpingTime > PUMPING_TIME_LIMIT) {
      stopWatering();
      return;  
    }    
    
    if (isPumping) {
      pumpingTime = pumpingTime + 2;
      Serial.println("Still pumping, incrementing the counter");
    } else {
      Serial.println("Plant is dry, start watering");
      wateredCount++;
      startWatering();
    }
    
  } else {
    stopWatering();
    Serial.println("Plant is OK");
  }
  
  delay(2000);
}

void startWatering() {
    digitalWrite(PUMP, HIGH);
    isPumping = true;
    pumpingTime = 0;
}

void stopWatering() {
    digitalWrite(PUMP, LOW);
    isPumping = false;
}

void refreshData() {
  
  int chk = DHT.read11(DHT11_PIN);
  
  char temperature[3];
  dtostrf(DHT.temperature, 2, 0, temperature);
  char humidity[3];
  dtostrf(DHT.humidity, 2, 0, humidity);
  
  display.clearDisplay();
  
  display.setCursor(0, 5);
  display.print("Temp: ");
  display.print(temperature);
  
  display.setCursor(0, 20);
  display.print("Humidity: ");
  display.print(humidity);   
   
  display.setCursor(0, 35);
  display.print("Count: ");
  display.print(wateredCount);   
                   
  display.display();
}

boolean isDry() {
  return digitalRead(SENSOR);
}
