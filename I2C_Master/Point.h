/* Point Object
 * The point stores cartesian coordinates with the
 * ability to convert from polar into the cartesian
 * coordinate system.
 */

#ifndef Point_h
#define Point_h

#include "Arduino.h"

struct Point {
public:
  Point() { x = 0; y = 0; }
	Point(int x, int y) { this->x = x; this->y = y; }
	static Point Polar(int r, int a) {
		return Point(r * cos(a), r * sin(a));
	}
  Point(const Point& other) {
    this->x = other.x;
    this->y = other.y;
  }
	int x;
	int y;

  /*
   * Distance Functions
   * There are other possibilities that could work more efficiently for
   * approximating the euclidean distance with less expensive operations
   * http://www.flipcode.com/archives/Fast_Approximate_Distance_Functions.shtml
   */
  static float manhattanDist(const Point& p1, const Point& p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
  }

  static float euclidDist2(const Point& p1, const Point& p2) {
    return pow(abs(p1.x - p2.x), 2) + pow(abs(p1.y - p2.y), 2);
  }

  static float euclidDist(const Point& p1, const Point& p2) {
    return sqrt(euclidDist2(p1, p2));
  }

  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }

  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }

//  ** Equality operator not working properly and may not be needed
//  Point operator=(const Point& other) {
//    return Point(other);
//  }
  
//	bool operator==(const Point& other) {
//		return x == other.x && y == other.y;
//	}

  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }

  String repr() const {
    String b = "(";
    String e = ")";
    String c = ", ";
    return b + x + c + y + e;
  }
};

#endif
