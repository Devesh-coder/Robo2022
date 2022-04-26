#define ENCA 2
#define ENCB 3
#define MOT 9
#define PWM 5

int pos = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
}

void setMotor(int dir, int pwmVal, int pwm, int in) {
  analogWrite(pwm, pwmVal);
  if(dir==1) {
    digitalWrite(in, LOW);
  }
  else if(dir==-1) {
    digitalWrite(in, HIGH);
  }
}

void readEncoder() {
  int b = digitalRead(ENCB);
  if(b>0) pos++;
  else pos--;
}

void loop() {
  // put your main code here, to run repeatedly:
  setMotor(1, 80, PWM, MOT);
  Serial.println(pos);
}
