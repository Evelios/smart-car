/*
 * Data Structures
 */

/* 
 * Point Class for polar and rect coordinates
 */
class Point {
  int X;
  int Y;
  int R;
  int A;
  int NormX;
  int NormY;
  
  Point(int x, int y) {
    X = x;
    Y = y;
    NormX = 400 + X/2;
    NormY = 800 - Y/2;
  }
  float distance(Point other) {
    return sqrt( pow(X - other.X, 2) + pow(Y - other.Y, 2) ); 
  }
};
//------------------------------------

//ArrayList<Point> points = new ArrayList<Point>();

import processing.serial.*;
//import cc.arduino.*;

//Arduino arduino;

Serial arduinoPort;
int height = 800;
int width = 800;

void setup() {
  size(800, 800);
  background(0);
  
  printArray(Serial.list());
  arduinoPort = new Serial(this, Serial.list()[2], 115200);
}

Point lastPoint = new Point(0, 0);

void draw() {
  //frameRate(30);
  
  while (arduinoPort.available() > 0) {
    String inString = arduinoPort.readString();
    String[] xyString = splitTokens(inString);
    if (xyString.length == 2) {
      
      int x = int(xyString[0]);
      int y = int(xyString[1]);
      
      Point point = new Point(x, y);
      
      print(point.X + "\t" + point.Y + "\n");
      
      // Throw out bad points
      if (abs(point.X) > 5 && abs(point.Y) > 5){
      
        // Fill some % and print new point
        fill(0, 8);
        rect(0, 0, 800, 800);
        
        // Fill in barrier blocks
        stroke(255, 0, 0, 10);
        fill(255, 0, 0);
        int res = 50;
        int x1 = point.NormX - point.NormX % res;
        int y2 = point.NormY - point.NormY % res;
        rect(x1, y2, res, res);
        
        // Fill in Point Information
        stroke(0, 255, 0);
        fill(0, 255, 0);
        
        // Fill in current point
        ellipse(point.NormX, point.NormY, 4, 4);
        
        
        // Connecting Line
        // If magnitude < distance draw barrier, else free space
        if ( point.distance(lastPoint) < 100) {
          line(lastPoint.NormX, lastPoint.NormY, point.NormX, point.NormY);
        }
        
        lastPoint = point;
      }
    }
  }
}