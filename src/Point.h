// Point.h

#ifndef POINT_H

#define POINT_H

struct Point2 {
  float x;
  float y;
  Point2(float x, float y): x(x), y(y) {}
};

struct Point3 {
  float x;
  float y;
  float z;
  Point3(float x=0, float y=0, float z=0): x(x), y(y), z(z) {}
};

#endif
