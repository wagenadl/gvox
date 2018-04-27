// Projection.h

#ifndef PROJECTION_H

#define PROJECTION_H

#include <QImage>

class Projection {
public:
  Projection(class Voxmap const *voxmap, class IDmap const *idmap);
  QImage flatids(int ax=1, int dir=1, int xdir=1, int ydir=1);
  QImage overlay(int ax=1, int dir=1, int xdir=1, int ydir=1);
  QImage projection(int ax=1, int dir=1, int xdir=1, int ydir=1);
  // ax=0,1,2 for x/y/z; dir<0 for backward view;
  // xdir<0 for hor. flip, ydir<0 for vert. flip
private:
  Voxmap const *voxmap;
  IDmap const *idmap;
};

#endif
