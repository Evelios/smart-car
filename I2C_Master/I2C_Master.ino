#include <Wire.h>
#include "List.h"
#include "Point.h"
#include "SearchProblemI2C.h"

MasterI2C master = MasterI2C(9);
List<Point> list;
Point pos;
Point goal;

void setup() {
  // Start the I2C Bus as Master
  Serial.begin(115200);

  list = List<Point>();
  list.push_back(Point(3, 5));
  list.push_back(Point(5, -7));
  list.push_back(Point(-23, 23));
  list.push_back(Point(3, 14));
  list.push_back(Point(-16, -4));
}

void loop() {
  Serial.println("Request info");
  master.request();
  pos = master.pos;
  goal = master.goal;
  Serial.println(pos.repr());
  Serial.println(goal.repr());
  
  master.sendPointList(list);
  delay(1000);
}


