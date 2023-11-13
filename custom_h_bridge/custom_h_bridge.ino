const int LowSwitch = 13;        // Replace with the actual pin number for LowSwitch
const int HighSwitch = 12;       // Replace with the actual pin number for HighSwitch
const int MotorDirectionPin1 = 11; // Replace with the actual pin number for motor direction control 1
const int MotorDirectionPin2 = 10; // Replace with the actual pin number for motor direction control 2
int delayVar = 100;
bool runningFunction = false;



void startMotorSwitch(){
  digitalWrite(LowSwitch, HIGH); // Logic Switch
  delay(delayVar);
  digitalWrite(HighSwitch, HIGH); //Power Mosfet Switch
  Serial.println("---High-Low Switch On");
}

void stopMotorSwitch(){
  digitalWrite(HighSwitch, LOW);
  delay(delayVar);
  digitalWrite(LowSwitch, LOW);
  Serial.println("---High-Low Switch Off");
}

void forward() {
  startMotorSwitch();
  // Set the motor direction pins according to your setup
  digitalWrite(MotorDirectionPin1, HIGH);
  digitalWrite(MotorDirectionPin2, LOW);
  Serial.println("Motor Forward");
}

void backward() {
  startMotorSwitch();
  // Set the motor direction pins according to your setup
  digitalWrite(MotorDirectionPin1, LOW);
  digitalWrite(MotorDirectionPin2, HIGH);
  Serial.println("Motor Backward");
}

void stop() {
  stopMotorSwitch();
  digitalWrite(MotorDirectionPin1, LOW);
  digitalWrite(MotorDirectionPin2, LOW);
  Serial.println("Motor Stop");
}



void setup() {
  pinMode(LowSwitch, OUTPUT);
  pinMode(HighSwitch, OUTPUT);
  pinMode(MotorDirectionPin1, OUTPUT);
  pinMode(MotorDirectionPin2, OUTPUT);

  Serial.begin(9600); // Set the baud rate for serial communication
}



void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if(command == 'f'){
      if(runningFunction == false){
        forward();
        runningFunction = true;
      }else{
        stop();
        forward();
        runningFunction = false;
      }
    }

    if(command == 'b'){
      if(runningFunction == false){
        backward();
        runningFunction = true;
      }else{
        stop();
        backward();
        runningFunction = false;
      }
    }

    if(command == 's'){
        runningFunction = false;
        stop();
    }

 
  }
}


