

#include <ESP32Servo.h>


Servo ESC; 
Servo myservo2;
Servo myservo3;
Servo myservo4;


int servo2pin=14;
int servo2Angle =90;

int servo3pin=27;
int servo3Angle =90;

int servo4pin=35;
int servo4Angle =90;



char receivedChar;

void setup() {

  Serial.begin(115200);
  ESC.attach(32);
  myservo2.attach(servo2pin);
  myservo3.attach(servo3pin);
  myservo4.attach(servo4pin);
}

void loop() {

   if (Serial.available()){
    receivedChar= Serial.read();
   }

    if(receivedChar=='x'){
      BLDC();
    }
    if(receivedChar=='s'){
      ESC.write(0);
    }

  if(receivedChar=='A'){
    FLY();
    }
  if(receivedChar=='x'){
      BLDC();
    }
  

  else if(receivedChar=='B'){
    LAND();
    }

else if(receivedChar=='R'){
  RIGHT();
}

else if(receivedChar=='L'){
  LEFT();
}
   
}//loop

void BLDC(){
  for(int i=0; i<=130; i++){
    ESC.write(i);
    delay(20);
  }
  for(int i=130; i>=30; i--){
    ESC.write(i);
    delay(20);
  }
  Serial.print("Bldc on");
delay(2000);
  ESC.write(2000);
}

void FLY(){
  myservo2.write(0);
  myservo3.write(0);
  myservo4.write(0);
  Serial.println("FLying");
}

void RIGHT(){
  myservo2.write(0);
  myservo3.write(180);
  myservo4.write(90);
  Serial.println("RightTurning");
}

void LEFT(){
  myservo2.write(180);
  myservo3.write(0);
  myservo4.write(90);
  Serial.println("LeftTurning");
}

void LAND(){
  myservo2.write(180);
  myservo3.write(180);
  myservo4.write(180);
  Serial.println("Landing");
}




