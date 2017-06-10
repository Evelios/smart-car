/* Class Search Problem
 * 
 * This class defines the search problem for the automated
 * car driven by lidar. The barriers are given by square
 * chunks and the goal is to find a route that can drive
 * through the terrain without hitting the obstacles to
 * arrive at the predetermined destination. This class
 * makes the assumption that the car cannot travel through
 * the diagonal.
 */

#ifndef CarSearchProblem_h
#define CarSearchProblem_h

#include "Arduino.h"
#include "List.h"
#include "GraphSearch.h"
#include "Point.h"

using namespace std;

/*
 * Problem is the abstract base class which is a template class
 * Problem<State, Action>, where State -> Point, and Action -> Point
 */
class CarSearchProblem : public Problem<Point, Point> {
public:
	CarSearchProblem(List<Point> barriers, Point initial, Point goal, int resolution);
	List<Point> actions(const Point& state) const;
	Point result(const Point& state, const Point& action) const;
  float heuristic(const Point& state, const Point& action) const;
	bool goalTest(Point& state);
	float pathCost(float c, const Point& state1, const Point& action, const Point& state2);

	List<Point> barriers;
  int resolution;
  List<Point> directions;
};

#endif
