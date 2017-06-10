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
  static float dist(const Point& p1, const Point& p2) {
    return euclidDist(p1, p2);
  }
   
  static float manhattanDist(const Point& p1, const Point& p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
  }

  static float euclidDist2(const Point& p1, const Point& p2) {
    return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
  }

  static float euclidDist(const Point& p1, const Point& p2) {
    return sqrt(euclidDist2(p1, p2));
  }

  /*
   * Mathematical Operations
   */

  static int dot(const Point& p1, const Point& p2) {
    return (p1.x * p2.x) + (p1.y * p2.y);
  }

  /*
   * Returns the cross product between two points
   * represeented as the magnitude in the k direction
   */
  static int cross(const Point& p1, const Point& p2) {
    return p1.x * p2.y - p1.y * p2.x;
  }

  /*
   * Returns the angle between two vectors in radians
   */
  static float angle(const Point& p1, const Point& p2) {
    return acos(dot(p1, p2) / (p1.mag() * p2.mag()));
  }

  float mag() const {
    return sqrt(x*x + y*y);
  }

  Point unit() const {
    return *this / mag();
  }

  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }

  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }

  Point operator*(float m) const {
    return Point(x * m, y * m);
  }

  Point operator/(float d) const {
    return Point(x / d, y / d);
  }

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
