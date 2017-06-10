#include "CarSearchProblem.h"

#include "Arduino.h"
#include "List.h"

#include "Point.h"

using namespace std;

/*
 * Initialize the search problem with the environment
 * barriers, starting position, and the ending goal.
 */
CarSearchProblem::CarSearchProblem(List<Point> barriers, Point initial, Point goal, int resolution)
  :Problem(initial, goal) {
	this->barriers = barriers;
  this->resolution = resolution;

  this->directions = List<Point>();
  this->directions.push_back(Point(0, resolution));
  this->directions.push_back(Point(0, -resolution));
  this->directions.push_back(Point(-resolution, 0));
  this->directions.push_back(Point(resolution, 0));
}


/* 
 * Return all the possible actions that can be performed
 * from the input state.
 */
List<Point> CarSearchProblem::actions(const Point& state) const {
	List<Point> possibleActions = List<Point>();

  auto dir_itr = directions.begin();
    while(dir_itr != directions.end()) {
    Point nextState = state + *dir_itr;
    if (!listContains(barriers, nextState)) {
      possibleActions.push_back(*dir_itr);
    }
    dir_itr++;
  }
  
  return possibleActions;
}

/* 
 * Return the state that results from executing the
 * action on the given input state. Action must be from
 * one of the actions(state) fucntion.
 */
Point CarSearchProblem::result(const Point& state, const Point& action) const {
//  Serial.print(state.repr());
//  Serial.print(action.repr());
//  Serial.println((state + action).repr());
	return state + action;
}

/*
   * Return the heuristic cost of reaching the goal state from the
   * current state with action. The heuristic could be a relaxed version
   * of the problem to give the algorithm a guiding direction towards
   * the goal node through the search space.
   */
float CarSearchProblem::heuristic(const Point& state, const Point& action) const {
  return Point::manhattanDist(this->goal, state);
}

/* 
 * Return true if the current state is in the goal state.
 * The basic implemetation is checking the current state
 * against the goal state.
 */
//bool CarSearchProblem::goalTest(Point& state) {
//	return goal == state;
//}

/* 
 * Returns the cost of a solution path which arrives at
 * state2 from state1 that resulted from taking action.
 * This result is asuming that it takes c cost to get to
 * state1. The default case is returning c + 1
 */
float CarSearchProblem::pathCost(float c, const Point& state1, const Point& action, const Point& state2) {
	return c + 1;
}
