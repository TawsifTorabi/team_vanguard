// Define the pins for motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Define the pins for motor B
int motor2Pin1 = 32;
int motor2Pin2 = 33;
int enable2Pin = 25;

#define ONBOARD_LED 2

// Define motor speed variables (0-255 for PWM)
int motor1Speed = 150;  // Adjust this value for desired speed of motor A
int motor2Speed = 150;  // Adjust this value for desired speed of motor B

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

void setup() {
  Serial.begin(9600);
  Dabble.begin("MyEsp32");

  pinMode(ONBOARD_LED, OUTPUT);

  // Set the pins for motor1 as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // Set the pins for motor2 as outputs:
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // Initialize the motor enable pins with PWM control
  analogWrite(enable1Pin, motor1Speed);
  analogWrite(enable2Pin, motor2Speed);
}

void loop() {
  Dabble.processInput();

  boolean a = GamePad.isUpPressed();
  boolean b = GamePad.isDownPressed();
  boolean c = GamePad.isLeftPressed();
  boolean d = GamePad.isRightPressed();
  boolean e = GamePad.isTrianglePressed();
  boolean f = GamePad.isCirclePressed();
  boolean g = GamePad.isCrossPressed();
  boolean h = GamePad.isSquarePressed();

  // Go forward
  if (a || e) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    motor1Speed = 150;  // Adjust this value for desired speed
    motor2Speed = 150;  // Adjust this value for desired speed
  }
  // Go Left condition
  else if (d || f) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    motor1Speed = 100;  // Adjust this value for desired speed
    motor2Speed = 100;  // Adjust this value for desired speed
  }
  // Right condition
  else if (c || h) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    motor1Speed = 100;  // Adjust this value for desired speed
    motor2Speed = 100;  // Adjust this value for desired speed
  }
  // Go back condition
  else if (b || g) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    motor1Speed = 100;  // Adjust this value for desired speed
    motor2Speed = 100;  // Adjust this value for desired speed
  }
  // Stop condition
  else {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    motor1Speed = 0;  // Stop motor A
    motor2Speed = 0;  // Stop motor B
  }

  // Update motor speeds using analogWrite
  analogWrite(enable1Pin, motor1Speed);
  analogWrite(enable2Pin, motor2Speed);

  digitalWrite(ONBOARD_LED, HIGH);
  delay(200);
  digitalWrite(ONBOARD_LED, LOW);
  delay(200);
}
