int motorDigital[2] = {22,3};
int motorAnalog[2] = {2,4};
int newSpeed=0;
int curSpeed=0; 
//bool toRun= false;
#define clockwise HIGH
#define anticlock LOW

void setup() {

  for(int i=0; i<2; i++){
    pinMode(motorDigital[i],OUTPUT);
    pinMode(motorAnalog[i], OUTPUT); 
  }
  Serial.begin(9600);
  Serial.println("Enter speed");
}

void loop() {
    digitalWrite(motorDigital[0], !anticlock);
    digitalWrite(motorDigital[1], anticlock);
    
    if(newSpeed > curSpeed){
      for(int i=curSpeed; i<=newSpeed; i++){
        analogWrite(motorAnalog[0], i);
        analogWrite(motorAnalog[1], i);
        delay(10);
      }
    }
    else if(newSpeed < curSpeed ){
      for(int i=curSpeed; i>=newSpeed; i--){
        analogWrite(motorAnalog[0], i);
        analogWrite(motorAnalog[1], i);
        delay(10);
      }
    }
    
    curSpeed = newSpeed;

    if(Serial.available()){
      String str= Serial.readString();
      newSpeed = str.toInt();
    Serial.println(newSpeed);
    }
}
