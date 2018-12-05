// Point.h

#ifndef POINT_H

#define POINT_H

#include <QDebug>
#include <math.h>

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
  void report() const {
    qDebug() << x << y << z;
  }
  float length() const {
    return sqrt(x*x + y*y + z*z);
  }
  Point3 &operator/=(float f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
  }			       
  Point3 &operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
  }			       
};

inline Point3 operator-(Point3 const &a, Point3 const &b) {
  Point3 p;
  p.x = a.x - b.x;
  p.y = a.y - b.y;
  p.z = a.z - b.z;
  return p;
}

struct iPoint3 {
  uint16_t x, y, z;
  iPoint3(uint16_t x=0, uint16_t y=0, uint16_t z=0): x(x), y(y), z(z) {}
};

inline QDebug operator<<(QDebug d, Point3 const &p) {
  d << p.x << p.y << p.z;
  return d;
}


#endif
