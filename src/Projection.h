// Projection.h

#ifndef PROJECTION_H

#define PROJECTION_H

#include <QImage>

class Projection {
public:
  Projection(class Voxmap const *voxmap, class IDmap const *idmap);
  QImage flatids(int ax, int dir); // ax=0,1,2 for x/y/z; dir<0 for backward view
  QImage overlay(int ax, int dir); // ax=0,1,2 for x/y/z; dir<0 for backward view
private:
  Voxmap const *voxmap;
  IDmap const *idmap;
};

#endif
