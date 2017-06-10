/* Required Non-Standard Libraries
 *  LIDARLite - Garmin Lite v3 LIDAR Library 
 *  LinkedList - ivanseidel's Linked List Library
 *  Point - Self built polar and rect point class
 *  
 * Lidar Unit Hookup Pins
 *  Green Top Right by reset button
 *  Blue Second Top Right by reset button
 *  
 * Servo Motor Hookup Pins
 *  Control pin Digital 11 - Arduino Uno/Due
 *  5V VCC to Servo
 *  GND to Servo
 *  
 * Communication Protocol Pins
 *  GND Needs to be hooked to GND on slave module
 *  A4 Needs to be hooked to A4 on slave module
 *  A5 Needs to be hooked to A5 on slave module
 *  
 * Information regarding this module
 *  This module is responsible for reading the environment
 *  through the lidar sensor and compressing that data so that
 *  a pathfinding algorithm can be run to determine the route
 *  from the current position to the goal position.
 *  The current position and goal position are read in through
 *  serial communication from the other (slave) module.
 *  The resultant path is then sent back to the other module.
 *  This process is then repeated.
 */

#include <LIDARLite.h>

#include "CarSearchProblem.h"
#include "SearchProblemI2C.h"
#include "ServoSweep.h"
#include "List.h"
#include "Point.h"


// Lidar Variables
LIDARLite myLidarLite;
int lidarReading = 1;

// Servo
ServoSweep servo;
int servoPin = 11;
int servoRate = 15;

// Communication Protocols
MasterI2C master = MasterI2C(9);

// Internal Data Structures
int resolution = 50; //cm
Point carPos;
Point goalPos;

// Stores the box chunk locations of the points being
// scanned and the previous instance of the scan
List<Point> chunksBuffer = List<Point>();
List<Point> chunksStored = List<Point>();

void setup() {
  Serial.begin(115200); // Initialize serial connection to display distance readings
  Serial.println("Running Master Module");
  
  // Lidar Setup
  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  myLidarLite.configure(0); // Change this number to try out alternate configurations
  
  // Servo Setup
  servo = ServoSweep(servoPin, servoRate);
}

/*
 * Main Loop
 */
void loop() {
   
  bool sweepFinished = servo.update();
  int lidarDistance = readLidar();
//  int lidarDistance = 0;


  Point point = Point::Polar(lidarDistance, 180 - servo.pos);

  // Weed out invalid point readings
  if (point.x > resolution && point.y > resolution) {
    // Fetch current data from the car control module
    master.request();
  //  carPos = pointToGrid(master.pos);
    Point chunkedPoint = pointToGrid(point);//carPos + point);
  
    // Store the current reading if not already in buffer
    if (!chunksBuffer.contains(chunkedPoint)) {
      chunksBuffer.push_back(chunkedPoint);
    }
  }
  
  // Update the data structures every time the lidar completes a sweep
  if (sweepFinished) {
    processData();
  } 
}

/*
 * This function sends the new chunks to the stored chunks list
 * The stored chunks is then processed by running the search algorithm
 * The results of the search buffer are then sent to the slave module
 */
void processData() {
  Serial.println("Process Data");
  chunksStored = chunksBuffer;
  chunksBuffer = List<Point>();

  Serial.println("Environmental barriers...");
  printList(chunksStored);

  // Fetch current data from the car control module
  master.request();
//  carPos = pointToGrid(master.pos);
//  goalPos = pointToGrid(master.goal);
  carPos = pointToGrid(Point(0, 0));
  goalPos = pointToGrid(Point(350, 400));

  Serial.print("Starting Position: ");
  Serial.println(carPos.repr());
  Serial.print("Goal Position:     ");
  Serial.println(goalPos.repr());

  CarSearchProblem problem = CarSearchProblem(chunksStored, carPos, goalPos, resolution);

  List<Point> actions = breadthFirstSearch<Point, Point>(problem);

  Serial.println("Path to solution...");
  printList(actions);

  master.sendPointList(actions);
}

/*
 * Read from the lidar unit.
 * Returns (int):
 *  cm distance to an object from the lidar unit
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
 * Takes in a point object and locks it into the lower resolution grid
 */
Point pointToGrid(Point point) {
  int x = point.x - point.x % resolution;
  int y = point.y - point.y % resolution;
  return Point(x, y);
}

/*
 * Used for development to pring a list of points
 */
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

