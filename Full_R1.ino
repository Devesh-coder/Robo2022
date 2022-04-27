#include <SPI.h>
#include <PS3BT.h>   
USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

#define clockwise HIGH
#define anticlock LOW

int valve1= 53;
int valve2= 51;

int throwValve1=43;
int throwValve2 = 45;
bool valState = LOW;
int timer1 = 0;

int counter=0;

int motorSpeed = 0;

int throwingMotorDigital[2] = {6,30};
int throwingMotorAnalog[2] = {5,7};

int reMotorDigital[2][2];
int reMotorAnalog[2][2];
bool dirArray[2][2] = {{0,1},{0,1}}; // 0,1,0,1

int motorDigital[2][2] = {{15,3},{17,26}};  // south, north, west, east
int motorAnalog[2][2] = {{12,4},{13,2}};   // south, north, west, east

void setup() {
  Serial.begin(9600);
   if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
   }
   
   for(int i=0; i<2; i++){
    pinMode(throwingMotorDigital[i],OUTPUT);
    pinMode(throwingMotorAnalog[i], OUTPUT); 
  }
  
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);

  pinMode(throwValve1, OUTPUT);
  pinMode(throwValve2, OUTPUT);

  for(int i=0; i<2; i++){
    for(int j=0; j<2; j++){
      pinMode(motorDigital[i][j], OUTPUT);
      pinMode(motorAnalog[i][j], OUTPUT);
    }
  }
  
//  pinMode(upDownMotorD,OUTPUT);
//  pinMode(upDownMotorA,OUTPUT);
}

void loop() {
  Usb.Task();

  if(millis()-timer1 == 500)
  {
      digitalWrite(throwValve1, LOW);
      digitalWrite(throwValve2, LOW);
      digitalWrite(valve2, LOW);
      digitalWrite(valve1, valState); 
      digitalWrite(valve2, !valState);
  }
  if(millis()-timer1 == 1000)
  {
      digitalWrite(valve2, LOW);
      digitalWrite(valve1, LOW); 
  }
  
  if (PS3.PS3Connected) {


    // Bot Motion
    if(PS3.getAnalogButton(R2)<5 && PS3.getAnalogButton(L2)<5){
      if((PS3.getAnalogHat(LeftHatY)>117 && PS3.getAnalogHat(LeftHatY)<137)){
        for(int i=0; i<2; i++){
          analogWrite(motorAnalog[1][i], 0);
        }
      }
      if(PS3.getAnalogHat(RightHatX)>117 && PS3.getAnalogHat(RightHatX)<137){
          for(int j=0; j<2; j++){
          analogWrite(motorAnalog[0][j], 0);
          }
        }
      }

    int left_down = map(PS3.getAnalogHat(LeftHatY),0,117,80,20);
    int left_up = map(PS3.getAnalogHat(LeftHatY),137,225,20,80);
    int right_down = map(PS3.getAnalogHat(RightHatX),0,117,80,20);
    int right_up = map(PS3.getAnalogHat(RightHatX),137,225,20,80);
    int L2_speed = map(PS3.getAnalogButton(L2),0,225,0,50);
    int R2_speed = map(PS3.getAnalogButton(R2),0,225,0,50);

    if(PS3.getAnalogHat(LeftHatY)>137){
      for(int i=0; i<2; i++){
        analogWrite(motorAnalog[1][i],left_up );
        digitalWrite(motorDigital[1][i], dirArray[1][0]); // low bc polarity
      }
    }

    if(PS3.getAnalogHat(LeftHatY)<117){
      for(int i=0; i<2; i++){
        analogWrite(motorAnalog[1][i],left_down );
        digitalWrite(motorDigital[1][i], dirArray[1][1]); //high polarity
      }
    }

    if(PS3.getAnalogHat(RightHatX)>137){
      for(int i=0; i<2; i++){
        analogWrite(motorAnalog[0][i],right_up );
        digitalWrite(motorDigital[0][i], dirArray[0][0]); //high
      }
    }

    if(PS3.getAnalogHat(RightHatX)<117){
      for(int i=0; i<2; i++){
        analogWrite(motorAnalog[0][i],right_down );
        digitalWrite(motorDigital[0][i], dirArray[0][1]);//low
      }
    }

    if(PS3.getAnalogButton(R2)>5){
      for(int i=0; i<2; i++){
        analogWrite(motorAnalog[i][1],R2_speed );
        digitalWrite(motorDigital[i][1], !dirArray[i][1]); // high
      }
      for(int j=0; j<2; j++){
        analogWrite(motorAnalog[j][0],R2_speed );
        digitalWrite(motorDigital[j][0], !dirArray[j][0]); // low
      }
    } 

    if(PS3.getAnalogButton(L2)>5){
      for(int i=0; i<2; i++){
        analogWrite(motorAnalog[i][1],L2_speed );
        digitalWrite(motorDigital[i][1], dirArray[i][1]); // low
      }
      for(int j=0; j<2; j++){
        analogWrite(motorAnalog[j][0],L2_speed );
        digitalWrite(motorDigital[j][0], dirArray[j][0]);// high
      }
    }

    if (PS3.getButtonClick(PS)) {
      PS3.disconnect();
    }

    // Throwing mechanism
        
    digitalWrite(throwingMotorDigital[0], !anticlock);
    digitalWrite(throwingMotorDigital[1], !anticlock);
  
    if (PS3.getButtonClick(R1)) {
      if(motorSpeed == 0){
        motorStart();
      }
    }

    if (PS3.getButtonClick(L1)){
      if(motorSpeed == 245){
        motorStop();
      }
    }

    if (PS3.getButtonClick(CROSS)) {
      digitalWrite(throwValve2, LOW);
      valState = !valState;
      digitalWrite(throwValve1, valState);
      digitalWrite(throwValve2, !valState);
      timer1 = millis();
    }

    if (PS3.getButtonClick(PS)) {
      PS3.disconnect();
    }
  }
  
}

void motorStart(){
  for(int i=0; i<=245; i++){
    analogWrite(throwingMotorAnalog[0], i);
    analogWrite(throwingMotorAnalog[1], i);
    delay(10);
  }
  motorSpeed = 245;
}

void motorStop(){
  for(int i=245; i>=0; i--){
    analogWrite(throwingMotorAnalog[0], i);
    analogWrite(throwingMotorAnalog[1], i);
    delay(10);
  }
  motorSpeed = 0;
}
