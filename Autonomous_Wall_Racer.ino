// Autonomous Wall Racer
// Author: Buki Adeniji, buki . adeniji @ yahoo . com
// Date completed: December 26th, 2016

// INSPIRATION:
// Project Inspired by - http://letsmakerobots.com/node/40752
//
// Code Inspired by - http://www.instructables.com/id/Wall-Following-Robot-Car/
//    - Nicholas Finch - Wall following Robot Car, April 16, 2014


// This project transforms a basic RC toy car into a Self-driving, autonomous wall-racer.
// The toy RC vehicle is hacked and fitted with an Arduino Microcontroller and two distance sensors, 
//  one Wall Facing, and other Forward Facing.

// ::Variable declaration::

// Initialize Ultrasonic Sensors
int outputPinFront = 5; // Front-facing Ultrasonic Sensor
int inputPinFront = 6; // Front-facing Ultrasonic Sensor
int outputPinWall = 10; // Wall-facing Ultrasonic Sensor
int inputPinWall = 11; // Wall-facing Ultrasonic Sensor

// Initialize Motor driver
const int motor_steerA = 4; //Steering Motor Pins
const int motor_steerB = 2; //Steering Motor Pins
const int motor_driveA = 7; //Drive Motor Pins
const int motor_driveB = 8; //Drive Motor Pins
const int motor_drive_speed = 3; // PWM output for speed control
//CAUTION: if the car's wheels are driving or turning in the opposite directions expected, simply edit and reverse these pins as needed

//motor_drive_speed - Drive motor speed controlled by PWM 
int M_SPD_HIGH = 200;            // Motor speed: (255 MAX)
int M_SPD_MED = 150;            // Motor speed: (255 MAX)
int M_SPD_LOW = 100;            // Motor speed: (255 MAX)


//Initialize front and wall distances
int rangeFront = 0;
int rangeWall = 0;


void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  // Setup motors
  pinMode(motor_steerA, OUTPUT);
  pinMode(motor_steerB, OUTPUT);
  pinMode(motor_driveA, OUTPUT);
  pinMode(motor_driveB, OUTPUT);
  
  pinMode(motor_drive_speed, OUTPUT);
    
  //set-up the HC-SR04 Ultrasonic sensors
  pinMode(outputPinFront, OUTPUT);
  pinMode(inputPinFront, INPUT);
  digitalWrite(outputPinFront, LOW);
  
  pinMode(outputPinWall, OUTPUT);
  pinMode(inputPinWall, INPUT);
  digitalWrite(outputPinWall, LOW);
  
}//endVoid

// ::Distance Parameters::
// Edit as required to fit your environment
// distances are measured in cm.

int CloseWall = 30;  // close allowable distance in cm between the car and the wall
int tooCloseWall = 20;  // closest allowable distance in cm between the car and the wall
int tooFarWall = 60;    // farthest allowable distance in cm between the car and the wall
int tooCloseFront = 20; // closest allowable distance in cm in front of the car and any obstacle

//////////////////////////////////////////////////////////

void loop(){
//-------------Main Program  

Main:
  rangeFront = readRangeFront(); // measure distance between car and any obstacle ahead
  rangeWall = readRangeWall();   // measure distance between car and wall
  
  
// Debug Code. 
// Uncomment to test the distance sensors.
// Ensure to comment out the main code below this debugger.
//   Serial.print(rangeFront);
//   Serial.print(" Front");
//   Serial.println();
//   delay(500);
//   Serial.print(rangeWall);
//   Serial.print(" Wall");
//   Serial.println();
//   delay(500);

//NOTE: Uncomment the debug codes below for troubleshooting
 if (rangeFront <= tooCloseFront){
    drive_backward();
    if (rangeWall >= tooCloseWall){  // Ony reverse and turn right if there is enough space between car and wall
      //  Serial.print(" Drive Back");
      turn_right();
      //  Serial.print(" Right Turn");
      //  Serial.println();
    }
    delay(400);
//  Now go forward and turn left    
//    drive_forward();
//    turn_left();
//    delay(1000);
    goto Main;    
  }//endif  
  
  if(rangeWall >= tooCloseWall && rangeWall <= tooFarWall){
    drive_forward();
    no_turn();
//  Serial.print(" Drive Forward");
//  Serial.println();
    goto Main;
  }//endeif  
  
  if (rangeWall <= CloseWall){
    turn_left();
//  Serial.print(" Turn Left");
    drive_forward();
//  Serial.print(" Drive Forward");
//  Serial.println();
    goto Main;
  }//endif 
  
  if (rangeWall <= tooCloseWall){
    motor_stop();
    turn_left();
//  Serial.print(" Hard Turn Left");
    drive_forward();
//  Serial.print(" Drive Forward");
//  Serial.println();
    goto Main;
  }//endif 
  
  if (rangeWall >= tooFarWall){
    turn_right();     // If car is going too far away from right wall, out of the range configured above, turn right towards the wall
//  Serial.print(" Turn Right");
    drive_forward();
//  Serial.print(" Drive Forward");
//  Serial.println();
    goto Main;
  }//endif  
}//endVoid


// Functions for the Directions

void no_turn(){
  digitalWrite(motor_steerA,LOW);
  digitalWrite(motor_steerB,LOW);

}
void motor_stop(){
  digitalWrite(motor_driveA, LOW); 
  digitalWrite(motor_driveB, LOW); 
//  digitalWrite(motor_steerA, LOW);
//  digitalWrite(motor_steerB,LOW);
}

void drive_forward(){
  digitalWrite(motor_driveA, HIGH); 
  digitalWrite(motor_driveB, LOW);
  analogWrite(motor_drive_speed,M_SPD_MED); 
}

void drive_backward(){
  digitalWrite(motor_driveA, LOW); 
  digitalWrite(motor_driveB, HIGH); 
  analogWrite(motor_drive_speed,M_SPD_MED); 
}

void turn_left(){
  digitalWrite(motor_steerA, HIGH); 
  digitalWrite(motor_steerB, LOW);
}

void turn_right(){
  digitalWrite(motor_steerA, LOW); 
  digitalWrite(motor_steerB, HIGH); 
}

//Functions for the HC-SR04 Ultrasonic Sensor 

int readRangeFront(){
// Measure distance in front of car
  delay(10);
  digitalWrite(outputPinFront, LOW);   // 
  delayMicroseconds(2);
  digitalWrite(outputPinFront, HIGH);  // 
  delayMicroseconds(10);
  digitalWrite(outputPinFront, LOW);    // 
      
  float duration = pulseIn(inputPinFront, HIGH);  // 
  float rangeFront= duration/29/2;       // 
      
//  Serial.print("Front distance:");       //
//  Serial.println(rangeFront);         //

  return rangeFront;
}

int readRangeWall(){
//Measure distance between car and wall on the right
  delay(10);
  digitalWrite(outputPinWall, LOW);   // 
  delayMicroseconds(2);
  digitalWrite(outputPinWall, HIGH);  // 
  delayMicroseconds(10);
  digitalWrite(outputPinWall, LOW);    // 
      
  float duration = pulseIn(inputPinWall, HIGH);  // 
  float rangeWall= duration/29/2;       // 
      
//  Serial.print("Wall distance:");       //
//  Serial.println(rangeWall);         //
  
  return rangeWall;
}
