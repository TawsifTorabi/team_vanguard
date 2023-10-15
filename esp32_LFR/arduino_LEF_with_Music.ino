#include <avr/sleep.h>
// Pin Definitions for L293 Motor Controller
const int ENA = 9; // PWM capable
const int ENB = 11;  // PWM capable
const int IN1 = 8;
const int IN2 = 7;
const int IN3 = 10;
const int IN4 = 12;

// Pin Definitions for IR Sensors
const int sensorPins[] = {2, 3, 4, 5, 6};
const int numSensors = 5;

// Variables
unsigned long lastLineDetectionTime = 0;
const unsigned long lineDetectionDelay = 2000; // 2 second

void initializeMotors() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void initializeIRSensors() {
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

int readSensors() {
  int sensorValues = 0;
  for (int i = 0; i < numSensors; i++) {
    // Adjust the logic based on the sensors providing LOW (0V) for black lines
    if (digitalRead(sensorPins[i]) == LOW) {
      sensorValues |= 1 << i;
    }
  }
  return sensorValues;
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void turnLeftOne() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRightOne() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void song(int buzzerPin){
  
  tone(buzzerPin, 294);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 392);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 659);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 392);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 659);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 523);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 523);
  delay(155);
  noTone(buzzerPin);
}

void endSong(int buzzerPin){
  
  tone(buzzerPin, 784);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 587);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 494);
  delay(155);
  noTone(buzzerPin);

  tone(buzzerPin, 392);
  delay(155);
  noTone(buzzerPin);

  sleep_mode();
}


void setup() {
  initializeMotors();
  initializeIRSensors();
  setMotorSpeed(130, 130); // Set initial motor speed
  song(13);
  // see below for sleep modes
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
  int sensorValues = readSensors();

  // Implement logic to control the motors based on sensor readings
  // You can use the sensorValues to make decisions about which direction to move

  if (sensorValues & 0b00010) {
    // First sensor detects the line (90-degree turn detected)
    turnLeft();
    setMotorSpeed(90, 90); // Set motor speed for a left turn
    lastLineDetectionTime = millis(); // Update the last line detection time
  }else if(sensorValues & 0b00001 || sensorValues & 0b00011){
    turnLeftOne();
    setMotorSpeed(90, 90); // Set motor speed for a left turn
    lastLineDetectionTime = millis(); // Update the last line detection time
  }else if(sensorValues & 0b10000 || sensorValues & 0b11000){
    turnRightOne();
    setMotorSpeed(90, 90); // Set motor speed for a left turn
    lastLineDetectionTime = millis(); // Update the last line detection time
  } else if (sensorValues & 0b01000) {
    // Fifth sensor detects the line (90-degree turn detected)
    turnRight();
    setMotorSpeed(90, 90); // Set motor speed for a right turn
    lastLineDetectionTime = millis(); // Update the last line detection time
  } else if (sensorValues & 0b00100 || sensorValues & 0b01110) {
    // Middle sensor detects the line
    moveForward();
    setMotorSpeed(130, 130); // Set motor speed for forward movement
    lastLineDetectionTime = millis(); // Update the last line detection time
  } else if (sensorValues & 11111) {
    // Finish Line Detected
    stopMotors();
    
    
    lastLineDetectionTime = millis(); // Update the last line detection time
  }else if (millis() - lastLineDetectionTime < lineDetectionDelay) {
    // Continue in the last direction for 1 second if no line is detected
    // You can adjust the delay as needed
    // You may also set the motors to stop after this time period
  } else {
    stopMotors(); // Stop if none of the conditions are met
    endSong(13);
  }

  delay(100); // Adjust the delay as needed to control the robot's speed
}
