/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(115200);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  // 22 - 175 - 95


  // if there's any serial available, read it:
  if (Serial.available() > 0) {
    processMessage();
  } else {
   // Serial.println("READY");
   // delay(500);
  }

/*  
  for (pos = 22; pos <= 175; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 175; pos >= 22; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
*/
}

void processMessage() {
  while (Serial.available() > 0) {

    char c = Serial.peek();
    // ignore newlines
    if (c == 'S') {
      Serial.println("Servo");
      Serial.read();
      int pos = Serial.parseInt();
      myservo.write(pos);
    } else {
      Serial.read(); 
    }
  }
}

