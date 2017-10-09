// Point.h

#ifndef POINT_H

#define POINT_H

#include <QDebug>

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
  void report() {
    qDebug() << x << y << z;
  }
};

#endif
