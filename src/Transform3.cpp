// Transform3.cpp

#include "Transform3.h"
#include <math.h>

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
  *this = *this * shifter(-x0, -y0, 0);
  *this = *this * xrotator(dxz);
  *this = *this * yrotator(dyz);
  *this = *this * shifter(x0, y0, 0);
}

