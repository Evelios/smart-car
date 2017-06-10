#include "SearchProblemI2C.h"
#include <Wire.h>
#include "List.h"
#include "Point.h"

SlaveI2C slave = SlaveI2C(9);
Point pos = Point(-120, 75);
Point goal = Point(447, -209);

void setup() {

  // Start the I2C Bus as Slave
  Serial.begin(115200);
  Serial.println("Running Slave Module");
}

void loop() {
  slave.update(pos, goal);
  
  if (slave.receivedMessage) {
    Serial.print("Received Data: ");
    printList(slave.input);
    slave.receivedMessage = false;
  }
  
  delay(1000);
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
