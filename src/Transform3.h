// Transform3.h

#ifndef TRANSFORM3_H

#define TRANSFORM3_H

#include "Point.h"
#include <QDebug>

class Transform3 {
  // maps screen coords to voxel coords
public:
  Transform3();
  Point3 apply(Point3 const &a) const {
    Point3 p;
    p.x = m[0][0]*a.x + m[0][1]*a.y + m[0][2]*a.z + m[0][3];
    p.y = m[1][0]*a.x + m[1][1]*a.y + m[1][2]*a.z + m[1][3];
    p.z = m[2][0]*a.x + m[2][1]*a.y + m[2][2]*a.z + m[2][3];
    return p;
  }
    
  void shift(float dx, float dy, float dz); // screen coords
  void rotate(float dxz, float dyz, float x0, float y0);
  void rotatez(float dxy, float x0, float y0);
  void scale(float s, float x0, float y0);
  void report() const;
  Transform3 inverse() const;
  double det() const;
public:
  static Transform3 shifter(float dx, float dy, float dz);
  static Transform3 yrotator(float dxz);
  static Transform3 xrotator(float dyz);
  static Transform3 zrotator(float dxy);
  static Transform3 scaler(float s);
public:
  float m[4][4];
};

Transform3 operator*(Transform3 const &l, Transform3 const &r);

QDebug operator<<(QDebug, Transform3 const &);

#endif
