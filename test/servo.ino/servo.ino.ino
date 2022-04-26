#include <Servo.h>
#define servoCount 1
int pos = 0;
int angle = 0;
int servoPins[servoCount] = {29};

Servo servo[servoCount];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for(int i=0; i<servoCount; i++) {
    servo[i].attach(servoPins[i]);
    servo[i].write(0);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    angle = (Serial.readString()).toInt();
    for(int j=0; j<servoCount; j++) {
    for (pos = 0; pos <= angle; pos += 1) {
        servo[j].write(pos);
      }
  }
      
//      delay(500);
//      
//      for (pos = 180; pos >= 0; pos -= 1) {
//        servo[j].write(pos);
//        delay(15);
//      }
  }
}
