/*  Wrapper for I2C communication for the Car Search problem
 *  This class has several requirements and pin dependencies 
 *  Arduino Uno:
 *    A4 and A5 used for I2C communication
 *  
 *  This protocol is based of the following motto
 *  Master:
 *    ask slave for the following info
 *      -> Current Position
 *      -> Goal Positions
 *    module runs search algorithm and sends the result info
 *    to the slave module
 *      -> The first 8 positions in the goal path [(x, y), ... ]
 *      
 *  Slave:
 *    on request send: 
 *      -> Current and goal position
 *    on receive:
 *      -> Retreive the starting path to the goal
 */

#ifndef SearchProblemI2C_h
#define SearchProblemI2C_h

#include <Wire.h>
#include "List.h"
#include "Point.h"

/* Sending and receiving integer values through I2C
 * http://forum.arduino.cc/index.php?topic=48477.0
 * Sending 16bit (2byte) integers
 */
const int MASK = 0xFF;

void sendInteger(int o) {
  int16_t out = (int16_t)o;
  char toSend = out & MASK;
  Wire.write(toSend);
  toSend = (out >> 8) & MASK;
  Wire.write(toSend);
}

/* Sending and receiving integer values through I2C
 * Sending 32bit (4byte) Points (x, y)
 */
void sendPoint(Point out) {
    sendInteger(out.x);
    sendInteger(out.y);
}

/* Attempts to receive an integer from an I2C request
 * then returns the integer value, returns 0 for invalid request
 */
int receiveInteger() {
  int16_t out = 0;
  if (Wire.available()) {
    char received = Wire.read();
    out = received;
  }
  if (Wire.available()) {
    char received = Wire.read();
    out |= (received << 8);
  }
  return (int)out;
}

/* Attempts to receive an integer from a current request
 * then returns the integer value, returns (0, 0) for invalid request
 */
Point receivePoint() {
  if (Wire.available()) {
    int x = receiveInteger();
    int y = receiveInteger();
    return Point(x, y);
  }
  return Point();
}

/*
 * Master Protocols
 */

class MasterI2C {
public:
  MasterI2C(int b) {
    bus = b;
    Wire.begin();
  }

  /*
   * Transmit the result back to the slave module
   * The transmission is a series of points (x, y)
   * which contain 16bit integer values
   * Transmission can hold 32bytes (8bits to 1byte)
   * this allows this transmission to send at most 8 points
   */
  void sendPointList(List<Point>& list) {
    Wire.beginTransmission(bus);

    int counter = 0;
    auto itr = list.begin();
    while (itr != list.end() && counter < 8) {
      Point p = *itr;
      sendPoint(p);   
      itr++;
      counter++;
    }

    Wire.endTransmission();
  }

  /*
   * Request two points from the slave module
   * Current position and the goal position
   * this transmission asks for 64bits (8bytes) of information
   * 
   */
  void request() {
    int bytesToRequest = 8;
    Wire.requestFrom(bus, bytesToRequest);
    pos = receivePoint();
    goal = receivePoint();
  }
  
  Point pos;
  Point goal;

private:
  int bus;
};

/*
 * Slave Protocols
 */

class SlaveI2C {
public:

  SlaveI2C(int b) {
    bus = b;
    input = List<Point>();
    SlaveI2C::classToUse = this;
  
    Wire.begin(bus);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
  }

  void update(Point p, Point g) {
    pos = p;
    goal = g;
  }

  static void receiveEvent(int bytes) {
    SlaveI2C::receivedMessage = true;
    classToUse->input.clear();
    while (Wire.available()) {
      classToUse->input.push_back(receivePoint());
    }
  }

  static void requestEvent() {
    sendPoint(classToUse->pos);
    sendPoint(classToUse->goal);
  }

  Point pos;
  Point goal;
  List<Point> input;
  static SlaveI2C* classToUse;
  static bool receivedMessage;
  
private:
  int bus;
};

SlaveI2C* SlaveI2C::classToUse = NULL;
bool SlaveI2C::receivedMessage = false;

#endif

