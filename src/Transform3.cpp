// Transform3.cpp

#include "Transform3.h"
#include <math.h>
#include <QDebug>

Transform3::Transform3() {
  for (int k=0; k<4; k++)
    for (int n=0; n<4; n++)
      m[k][n] = k==n;
}

Transform3 Transform3::shifter(float dx, float dy, float dz) {
  Transform3 mm;
  mm.m[0][3] = dx;
  mm.m[1][3] = dy;
  mm.m[2][3] = dz;
  return mm;
}

Transform3 Transform3::xrotator(float dxz) {
  Transform3 mm;
  mm.m[0][0] = cos(dxz);
  mm.m[2][2] = cos(dxz);
  mm.m[2][0] = sin(dxz);
  mm.m[0][2] = -sin(dxz);
  return mm;
}

Transform3 Transform3::yrotator(float dyz) {
  Transform3 mm;
  mm.m[1][1] = cos(dyz);
  mm.m[2][2] = cos(dyz);
  mm.m[2][1] = sin(dyz);
  mm.m[1][2] = -sin(dyz);
  return mm;
}

Transform3 Transform3::zrotator(float dxy) {
  Transform3 mm;
  mm.m[0][0] = cos(dxy);
  mm.m[1][1] = cos(dxy);
  mm.m[1][0] = sin(dxy);
  mm.m[0][1] = -sin(dxy);
  return mm;
}

Transform3 Transform3::scaler(float s) {
  Transform3 mm;
  mm.m[0][0] = s;
  mm.m[1][1] = s;
  mm.m[2][2] = s;
  return mm;
}

Transform3 operator*(Transform3 const &l, Transform3 const &r) {
  Transform3 res;
  for (int n=0; n<4; n++) {
    for (int m=0; m<4; m++) {
      float v = 0;
      for (int k=0; k<4; k++) 
	v += l.m[n][k] * r.m[k][m];
      res.m[n][m] = v;
    }
  }
  return res;
}

void Transform3::shift(float dx, float dy, float dz) {
  *this = *this * shifter(dx, dy, dz);
}

void Transform3::rotate(float dxz, float dyz, float x0, float y0) {
  // I want to rotate such that the mapping of (x0,y0,0) is unchanged.
  Point3 p0 = apply(Point3(x0, y0, 0));
  *this = *this * xrotator(dxz);
  *this = *this * yrotator(dyz);
  Point3 p1 = apply(Point3(x0, y0, 0));
  *this = shifter(p0.x-p1.x, p0.y-p1.y, p0.z-p1.z) * *this;
}

void Transform3::rotatez(float dxy, float x0, float y0) {
  // I want to rotate such that the mapping of (x0,y0,0) is unchanged.
  Point3 p0 = apply(Point3(x0, y0, 0));
  *this = *this * zrotator(dxy);
  Point3 p1 = apply(Point3(x0, y0, 0));
  *this = shifter(p0.x-p1.x, p0.y-p1.y, p0.z-p1.z) * *this;
}

void Transform3::scale(float s, float x0, float y0) {
  // I want to rotate such that the mapping of (x0,y0,0) is unchanged.
  Point3 p0 = apply(Point3(x0, y0, 0));
  *this = *this * scaler(s);
  Point3 p1 = apply(Point3(x0, y0, 0));
  *this = shifter(p0.x-p1.x, p0.y-p1.y, p0.z-p1.z) * *this;
}


void Transform3::report() {
  qDebug() << m[0][0] << m[0][1] << m[0][2] << m[0][3];
  qDebug() << m[1][0] << m[1][1] << m[1][2] << m[1][3];
  qDebug() << m[2][0] << m[2][1] << m[2][2] << m[2][3];
  qDebug() << m[3][0] << m[3][1] << m[3][2] << m[3][3];
}
