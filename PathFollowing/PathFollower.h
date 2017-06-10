#ifndef PathFollowing_h
#define PathFollowing_h

#include "Point.h"
#include "List.h"

class PathFollower {
public:  
  PathFollower() {
    path = List<Point>();
    pos = Point();
    vel = Point();
    currentGoal = Point();
    int minDistance = 50;
  }

  void update(const Point& p, const Point& v) {
    pos = p;
    vel = v;
    updatePath();
  }

  void addNode(const Point& p) {
    path.push_back(p);
  }

  void newPath(const List<Point>& points) {
    path = points;
    currentGoal = path.front();
  }

  /*
   * If the car drives within the minimum distance of the
   * current goal node remove that waypoint from the lsit
   * and set the next point as the goal position
   */
  void updatePath() {
    if (Point::dist(pos, currentGoal) < minDist) {
      path.pop_front();
      if (path.begin() != path.end()) {
        currentGoal = path.front();
      }
    }
  }

  /*
   * Returns (float):
   *  value that is range limited between -1 (left) and 1 (right) corresponding
   *  to the direction and magnitude that the car needs to be
   */
  float steering() const {
    Point seeking = currentGoal - pos;
    return sign(Point::cross(seeking, vel)) * Point::angle(vel, seeking) / PI;
  }

private:
  int minDist;
  Point pos;
  Point vel;
  Point currentGoal;
  List<Point> path;

  int sign(int x) const {
    if (x == 0) {
      return 0;
    } else if (x < 0) {
      return -1;
    } else {
      return 1;
    }
  }
};

#endif
