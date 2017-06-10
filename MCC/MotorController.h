#include <Servo.h>
#ifndef MotorController_h
#define MotorController_h

/*
  Device Status    EN D1 D2' IN1 IN2 OUT1 OUT2
  Forward          H  L   H   H   L   H    L
  Reverse          H  L   H   L   H   L    H
  Disable1         H  H   X   X   X   Z    Z
  Disable2         H  X   L   X   X   Z    Z
*/

class MotorController {
  public:
    MotorController() {}

    MotorController(int in1, int in2, int d1, int en, int st, int d2, int servoPower) {
      M1_IN1_Pin = in1;
      M1_IN2_Pin = in2;
      M1_D1_PWM_Pin = d1;
      EN_Pin = en;
      ServoPin = st;
      sp = servoPower;
      D2_Pin = d2;

      pinMode(M1_IN1_Pin, OUTPUT);   // sets the pins as output
      pinMode(M1_IN2_Pin, OUTPUT);
      pinMode(M1_D1_PWM_Pin, OUTPUT);
      pinMode(EN_Pin, OUTPUT);
      pinMode(sp, OUTPUT);
      pinMode(D2_Pin, OUTPUT);

      servo.attach(ServoPin);
    }

    void update() {
      updateMotor();
      // Update Servo
      servo.write(turnAng);
      Serial.println("Updating Servo");
    }

    void turn(int angChange) {
      Serial.println("Turning");
      turnAng += angChange;
      if (turnAng < 45) {
        turnAng = 45;
      } else if (turnAng > 135) {
        turnAng = 135;
      }
    }

    void disableMotor() {
      motorOutput = false;
    }

    void enableMotor() {
      motorOutput = true;
    }

    void moveForward() {
      motorForward = true;
    }

    void moveBackward() {
      motorForward = false;
    }

    int angle() {
      return turnAng;
    }

  private:

    // New UpdateMotor
    void updateMotor() {
      if (!motorOutput) {
       // Serial.println("OFF");
        digitalWrite(EN_Pin, 0);
      } else {
        digitalWrite(EN_Pin, 1);

        if (motorForward) {
          digitalWrite(M1_IN1_Pin, 1);
          digitalWrite(M1_IN2_Pin, 0);
        } else {
          digitalWrite(M1_IN1_Pin, 0);
          digitalWrite(M1_IN2_Pin, 1);
        }

        analogWrite(M1_D1_PWM_Pin, 255 - velocity);

        digitalWrite(D2_Pin, 1);
        digitalWrite(sp, 1);
//        Serial.println(velocity);
      }
    }

    
    int velocity = 200;      // variable to store the read value 0 to 255
    bool motorOutput = false;// Logic output to motor
    bool motorForward = true;// Logic output to motor
    int turnAng = 95;        // variable to store the servo position
    Servo servo;

    // Pin Storage
    int M1_IN1_Pin;
    int M1_IN2_Pin;
    int M1_D1_PWM_Pin;
    int EN_Pin;
    int ServoPin;
    int D2_Pin;
    int sp;

};

#endif
