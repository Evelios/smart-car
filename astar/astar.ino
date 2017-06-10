#include "List.h"
#include "GraphSearch.h"
#include "CarSearchProblem.h"
#include "Point.h"

#include <MemoryFree.h>


List<Point> barriers;
Point start;
Point goal;
int resolution = 1;

void setup() {

  Serial.begin(115200);

  Serial.println("Running Search Algorithm... \n");
  
  start = Point(0, 0);
  goal = Point(0 * resolution, 1 * resolution);

  barriers = List<Point>();
  barriers.push_back( Point(-2*resolution, 3 * resolution) );
  barriers.push_back( Point(-1*resolution, 3 * resolution) );
  barriers.push_back( Point(0            , 3 * resolution) );
  barriers.push_back( Point(resolution   , 3 * resolution) );

  Serial.print("Start: ");
  Serial.println(start.repr());
  Serial.print("Goal: ");
  Serial.println(goal.repr());

  Serial.print("Barriers: ");
  printList(barriers);

  delay(1000);
  
  

//  List<Point> actions = breadthFirstSearch<Point, Point>(problem);
//
//  Serial.print("Solution actions");
//  printList(actions);
}

void loop() {
  CarSearchProblem problem = CarSearchProblem(barriers, start, goal, resolution);

  List<Point> actions = breadthFirstSearch<Point, Point>(problem);

  Serial.print("Solution actions");
  printList(actions);

//  while(true) {}

    delay(250);
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

void printList(PriorityList<Point> list) {
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

void printList(PriorityList<int> list) {
  Serial.print("[ ");
  auto itr = list.begin();
  while (itr != list.end()) {
    Serial.print(*itr);
    itr++;
    if (itr != list.end())
      Serial.print(", ");
  }
  Serial.println(" ]");
}

