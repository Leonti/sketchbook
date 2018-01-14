
int senRead=0;

void setup() {
  // put your setup code here, to run once:

  // initialize serial:
  Serial.begin(115200);


}

void loop() {
/*
      
  if (Serial.available()) {
    byte byteRead = Serial.read();
    Serial.write(byteRead);
    Serial.write(byteRead);
    Serial.write(byteRead);
  }

*/
  delay(200);
  //Serial.write("test");
  
  int val=analogRead(senRead);  //variable to store values from the photodiode  
  Serial.println(val); 
}
