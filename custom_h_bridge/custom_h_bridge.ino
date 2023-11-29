//const int LowSwitch = 13;        // Replace with the actual pin number for LowSwitch
const int HighSwitch = 12;       // Replace with the actual pin number for HighSwitch
const int MotorDirectionPin1 = 11; // Replace with the actual pin number for motor direction control 1
const int MotorDirectionPin2 = 10; // Replace with the actual pin number for motor direction control 2
const int ACSPin = 14; //A0
const float CurrentLimitReading = 7.5;
int delayVar = 10;
bool runningFunction = false;
bool OverCurrent = false;



void startMotorSwitch(){
  //digitalWrite(LowSwitch, HIGH); // Logic Switch
  //delay(delayVar);
  if(!OverCurrent){
    digitalWrite(HighSwitch, HIGH); //Power Mosfet Switch
    Serial.println("---High-Low Switch On");
  }else{
    Serial.println("---DANGER! OVERCURRENT!");
  }
}

void stopMotorSwitch(){
  digitalWrite(HighSwitch, LOW);
  //delay(delayVar);
  //digitalWrite(LowSwitch, LOW);
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
  //pinMode(LowSwitch, OUTPUT);
  pinMode(HighSwitch, OUTPUT);
  pinMode(MotorDirectionPin1, OUTPUT);
  pinMode(MotorDirectionPin2, OUTPUT);
  pinMode(ACSPin, INPUT);

  Serial.begin(9600); // Set the baud rate for serial communication
}



void loop() {
  //Serial Commands Starts
  if (Serial.available() > 0) {
    char command = Serial.read();
    if(!OverCurrent){
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
  //Serial Available Commands Ends


  if(!OverCurrent){
    unsigned int x=0;
    float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;
     
    for (int x = 0; x < 150; x++){ //Get 150 samples
      AcsValue = analogRead(A0);     //Read current sensor values  
      Samples = Samples + AcsValue;  //Add samples together
      delay (3); // let ADC settle before next sample 3ms
    }
    AvgAcs=Samples/150.0;//Taking Average of Samples
     
    AcsValueF = abs((2.5 - (AvgAcs * (5.0 / 1024.0)) )/0.100);
  
    //
    Serial.println(AcsValueF);
    if(AcsValueF < CurrentLimitReading){
      OverCurrent = true;
      stop();
      Serial.println("Overcurrent Cut-off!+++++++");
    }
    
    //Serial.println(AcsValueF);//Print the read current on Serial monitor
    //Serial.println("Overcurrent: " + OverCurrent);
  }
  
}
