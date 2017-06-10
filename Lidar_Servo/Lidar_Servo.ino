/* Required Non-Standard Libraries
 *  LIDARLite - Garmin Lite v3 LIDAR Library 
 *  LinkedList - ivanseidel's Linked List Library
 *  Point - Self built polar and rect point class
 *  
 *  
 * Lidar Unit Hookup Pins
 *  Green Top Right by reset button
 *  Blue Second Top Right by reset button
 *  
 * Servo Motor Hookup Pins
 *  Control pin Digital 11 - Arduino
 *  Control pin Digital 20 - Teensy 3.2
 */

#include <LIDARLite.h>
#include <Servo.h>
#include "List.h"
#include "Point.h"

#include "CarSearchProblem.h"
#include "SearchProblemI2C.h"
#include "ServoSweep.h"


// Lidar Variables
LIDARLite myLidarLite;
int lidarReading = 1;

// Servo Variables
Servo servo;
//const int servoPin = 23; // Teensy 3.2
const int servoPin = 11; // Arduino Uno
const int updateDelay = 15;
int pos = 1;
unsigned long lastUpdateMs = 0;
int updateDirection = 1;

// Internal Data Structures
int resolution = 50;
List<Point> chunksBuffer = List<Point>();
List<Point> chunksStored = List<Point>();

void setup() {
  Serial.begin(115200); // Initialize serial connection to display distance readings
  
  // Lidar Setup
  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  myLidarLite.configure(0); // Change this number to try out alternate configurations
  
  // Servo Setup
  servo.attach(servoPin);
  lastUpdateMs = millis();
}

/*
 * Main Loop
 */
void loop() {
  bool sweepFinished = updateServo();
  int lidarDistance = readLidar();

  Point point = Point::Polar(lidarDistance, 180 - pos);
  Point chunkedPoint = pointToGrid(point);

  // Store the current reading if not already in buffer
  if (!chunksBuffer.contains(chunkedPoint)) {
    chunksBuffer.push_back(chunkedPoint);
  }

  // Update the data structures every time the lidar completes a sweep
  if (sweepFinished) {
    processData();
  }  

  if (lidarReading % 10 == 0) {

    // Cartesian
    Serial.print(point.x);
    Serial.print("\t");
    Serial.print(point.y);
    Serial.print("\n");

  }
  
}

void processData() {
  chunksStored = chunksBuffer;
  chunksBuffer = List<Point>();

  printList(chunksStored);
}

/*
 * Take in lidar readings 
 */
int readLidar() {
  if (lidarReading == 100) {
    lidarReading = 1;
    return myLidarLite.distance(); // Unbiased Reading
  } else {
    lidarReading++;
    return myLidarLite.distance(false); // Biased Reading
  }
}

/*
 * The servo sweeps from 
 */
bool updateServo() {
//  Serial.println(millis());
  if((millis() - lastUpdateMs) > updateDelay)  // time to update
  {
    lastUpdateMs = millis();
    pos += updateDirection;
    servo.write(pos);
    if ((pos >= 180) || (pos <= 0)) // end of sweep
    {
      // reverse direction
      updateDirection = -updateDirection;
      return true;
    }
  }
  return false;  
}

/*
 * Takes in a point object and locks it into the lower resolution grid
 */
Point pointToGrid(Point point) {
  int x = point.x - point.x % resolution;
  int y = point.y - point.y % resolution;
  return Point(x, y);
}

void printList(List<Point> list) {
  Serial.print("[ ");
  auto itr = list.begin();
  while (itr != list.end()) {
    Serial.print((*itr).repr());
    itr++;
    if (itr != list.end())
      Serial.print(", ");
  }
  Serial.println(" ]");
}

