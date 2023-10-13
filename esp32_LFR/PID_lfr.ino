#include <Arduino.h>
// IR Sensors
int sensorPins[] = {35, 32, 33, 25, 26};
int sensor[5] = {0, 0, 0, 0, 0}; // Initialize the sensor array

// Motor Variables
int ENA = 15;
int motorInput1 = 12;
int motorInput2 = 13;
int motorInput3 = 2;
int motorInput4 = 14;
int ENB = 4;

// Initial Speed of Motor
int initial_motor_speed = 140;

// Output Pins for Led
int ledPin1 = A3;
int ledPin2 = A4;

// PID Constants
float Kp = 25;
float Ki = 0;
float Kd = 15;

float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

int flag = 0;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);

  Serial.begin(9600);
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}

void loop() {
  read_sensor_values();
  Serial.print(error);

  if (error == 100) {  // Make left turn until it detects a straight path
    do {
      read_sensor_values();
      analogWrite(ENA, 110);
      analogWrite(ENB, 90);
      sharpLeftTurn();
    } while (error != 0);
  } else if (error == 101) {  // Make right turn if it detects only the right path
    analogWrite(ENA, 110);
    analogWrite(ENB, 90);
    forward();
    delay(200);
    stop_bot();
    read_sensor_values();
    if (error == 102) {
      do {
        analogWrite(ENA, 110);
        analogWrite(ENB, 90);
        sharpRightTurn();
        read_sensor_values();
      } while (error != 0);
    }
  } else if (error == 102) {  // Make left turn until it detects a straight path
    do {
      analogWrite(ENA, 110);
      analogWrite(ENB, 90);
      sharpLeftTurn();
      read_sensor_values();
      if (error == 0) {
        stop_bot();
        delay(200);
      }
    } while (error != 0);
  } else if (error == 103) {  // Make left turn until it detects a straight path or stop if a dead end is reached
    if (flag == 0) {
      analogWrite(ENA, 110);
      analogWrite(ENB, 90);
      forward();
      delay(200);
      stop_bot();
      read_sensor_values();
      if (error == 103) { // Dead End Reached, Stop!
        stop_bot();
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        flag = 1;
      } else { // Move Left
        analogWrite(ENA, 110);
        analogWrite(ENB, 90);
        sharpLeftTurn();
        delay(200);
        do {
          read_sensor_values();
          analogWrite(ENA, 110);
          analogWrite(ENB, 90);
          sharpLeftTurn();
        } while (error != 0);
      }
    }
  } else {
    calculate_pid();
    motor_control();
  }
}

void read_sensor_values() {
  for (int i = 0; i < 5; i++) {
    sensor[i] = !digitalRead(sensorPins[i]);
  }

  if (sensor[0] && !sensor[1] && !sensor[2] && !sensor[3] && !sensor[4])
    error = 4;
  else if (sensor[0] && sensor[1] && !sensor[2] && !sensor[3] && !sensor[4])
    error = 3;
  else if (!sensor[0] && sensor[1] && !sensor[2] && !sensor[3] && !sensor[4])
    error = 2;
  else if (!sensor[0] && sensor[1] && sensor[2] && !sensor[3] && !sensor[4])
    error = 1;
  else if (!sensor[0] && !sensor[1] && sensor[2] && !sensor[3] && !sensor[4])
    error = 0;
  else if (!sensor[0] && !sensor[1] && sensor[2] && sensor[3] && !sensor[4])
    error = -1;
  else if (!sensor[0] && !sensor[1] && !sensor[2] && sensor[3] && !sensor[4])
    error = -2;
  else if (!sensor[0] && !sensor[1] && !sensor[2] && sensor[3] && sensor[4])
    error = -3;
  else if (sensor[0] && sensor[1] && sensor[2] && sensor[3] && !sensor[4]) // Turn robot left side
    error = 100;
  else if (!sensor[0] && sensor[1] && sensor[2] && sensor[3] && sensor[4]) // Turn robot right side
    error = 101;
  else if (!sensor[0] && !sensor[1] && !sensor[2] && !sensor[3] && !sensor[4]) // Make U turn
    error = 102;
  else if (sensor[0] && sensor[1] && sensor[2] && sensor[3] && sensor[4]) // Turn left side or stop
    error = 103;
}

void calculate_pid() {
  P = error;
  I = I + previous_I;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_I = I;
  previous_error = error;
}

void motor_control() {
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);
  analogWrite(ENA, left_motor_speed);
  analogWrite(ENB, right_motor_speed - 30);
  forward();
}

void forward() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void reverse() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}

void right() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}

void left() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}

void sharpRightTurn() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void sharpLeftTurn() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}

void stop_bot() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}
