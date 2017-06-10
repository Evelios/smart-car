#ifndef ServoSweep_h
#define ServoSweep_h

#include <Servo.h>

class ServoSweep {
public:

  ServoSweep() { }

  ServoSweep(int pin, int rate) {
    servoPin = pin;
    updateDelay = rate;
    pos = 1;
    lastUpdateMs = 0;
    updateDirection = 1;

    servo.attach(servoPin);
    lastUpdateMs = millis();
  }
  
  bool update() {
    if((millis() - lastUpdateMs) > updateDelay) {  // time to update
      lastUpdateMs = millis();
      pos += updateDirection;
      servo.write(pos);
      if ((pos >= 180) || (pos <= 0)) { // End of sweep
        // reverse direction
        updateDirection = -updateDirection;
        return true;
      }
    }
    return false;
  }

  
  int pos;
  
private:
  Servo servo;
  int servoPin;
  unsigned long updateDelay;
  unsigned long lastUpdateMs;
  int updateDirection;
};

#endif
