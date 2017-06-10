#include "MotorController.h"

// Motor Controller Variables
MotorController motorControl;

int in1 = 7; //Blue Wire
int in2 = 6; //Purple Wire
int d1 = 5; //Orange Wire
int en = 4; //Gray Wire
int ServoPin = 3; //White Wire
int servoPower = 10; //Red Wire
int SteerChange = -10;

int d2 = 9;

// Speed Calculation Variables
const byte interruptPin = 13;
volatile long prevT;
float circum = 5 * PI;
volatile float speed = 0;

void setup()
{
  Serial.begin(115200);
  motorControl = MotorController(in1, in2, d1, en, ServoPin, d2, servoPower);
  motorControl.disableMotor();
  motorControl.moveForward();
  Serial.println("Motor Controller");

  // Speed Controller
  prevT = micros();
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}

void loop()
{  
  Serial.println(motorControl.angle());
  motorControl.turn(-10);
  motorControl.disableMotor();
  motorControl.update();
  delay(1500);
  
  motorControl.enableMotor();
  motorControl.moveForward();
  motorControl.update();
  delay(1500);
  
  motorControl.disableMotor();
  motorControl.update();
  delay(1500);
  
  //motorControl.turn(SteerChange);
  
  motorControl.enableMotor();
  motorControl.moveBackward();
  motorControl.update();
  delay(1500);
  
Serial.println(motorControl.angle());

}

void speedUpdate() {
  unsigned long delT = micros() - prevT;
  speed = (circum/4) / (delT/1000000.0);
  prevT = micros();
}

