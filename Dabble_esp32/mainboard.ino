// Here we have defined pins for motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Here we have defined pins for motor B
int motor2Pin1 = 32;
int motor2Pin2 = 33;
int enable2Pin = 25;

#define ONBOARD_LED 2

//defining custom settings, you need to install dabble library in your program

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>





/////////////////////
///////////////////////





void setup() {
Serial.begin(9600);
Dabble.begin("MyEsp32");

pinMode(ONBOARD_LED, OUTPUT);

//* setting the pins for motor1 as outputs:
pinMode(motor1Pin1, OUTPUT);
pinMode(motor1Pin2, OUTPUT);
pinMode(enable1Pin, OUTPUT);

// sets the pins for motor2 as outputs:
pinMode(motor2Pin1, OUTPUT);
pinMode(motor2Pin2, OUTPUT);
pinMode(enable2Pin, OUTPUT);

digitalWrite(enable1Pin, HIGH);
digitalWrite(enable2Pin, HIGH);
}

void loop() {

Dabble.processInput();

// It will be checking which button is pressed or not? Depending upon the button pressed, output pins of the microcontroller will be set to high or low. We are using else if statement.


boolean a = GamePad.isUpPressed();
boolean b = GamePad.isDownPressed();
boolean c = GamePad.isLeftPressed();
boolean d = GamePad.isRightPressed();
boolean e = GamePad.isTrianglePressed();
boolean f = GamePad.isCirclePressed();
boolean g = GamePad.isCrossPressed();
boolean h = GamePad.isSquarePressed();
// boolean i = GamePad.isStartPressed();
// boolean j = GamePad.isSelectPressed();



//Go forward

if (a || e){
digitalWrite(motor1Pin1, LOW);
digitalWrite(motor1Pin2, HIGH);
digitalWrite(motor2Pin1, HIGH);
digitalWrite(motor2Pin2, LOW);

Serial.write("Forward");

digitalWrite(ONBOARD_LED, HIGH);
delay(200);
digitalWrite(ONBOARD_LED, LOW);
delay(200);
}

// Go Left condition
else if(d || f){
digitalWrite(motor1Pin1, LOW);
digitalWrite(motor1Pin2, HIGH);
digitalWrite(motor2Pin1, LOW);
digitalWrite(motor2Pin2, LOW);


Serial.write("Left");

digitalWrite(ONBOARD_LED, HIGH);
delay(200);
digitalWrite(ONBOARD_LED, LOW);
delay(200);
}


//* Rights condition
else if(c || h){
digitalWrite(motor1Pin1, LOW);
digitalWrite(motor1Pin2, LOW);
digitalWrite(motor2Pin1, HIGH);
digitalWrite(motor2Pin2, LOW);

Serial.write("Right");

digitalWrite(ONBOARD_LED, HIGH);
delay(200);
digitalWrite(ONBOARD_LED, LOW);
delay(200);
}

// Go back condition

else if(b || g){
digitalWrite(motor1Pin1, HIGH);
digitalWrite(motor1Pin2, LOW);
digitalWrite(motor2Pin1, LOW);
digitalWrite(motor2Pin2, HIGH);

Serial.write("Back");

digitalWrite(ONBOARD_LED, HIGH);
delay(200);
digitalWrite(ONBOARD_LED, LOW);
delay(200);
}


//stop condition
else{
digitalWrite(motor1Pin1, LOW);
digitalWrite(motor1Pin2, LOW);
digitalWrite(motor2Pin1, LOW);
digitalWrite(motor2Pin2, LOW);

digitalWrite(ONBOARD_LED, HIGH);
delay(2000);
digitalWrite(ONBOARD_LED, LOW);
delay(2000);
}
}
