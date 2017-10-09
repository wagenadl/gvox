// Viewer.h

#ifndef VIEWER_H

#define VIEWER_H

#include "Transform3.h"
#include <QLabel>

class Viewer: public QLabel {
public:
  Viewer(QWidget *parent=0);
  void setVoxmap(class Voxmap *voxmap);
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);
  void resizeEvent(QResizeEvent *);
protected:
  void rebuild();
private:
  Transform3 t;
  int hidpi_;
  Voxmap *voxmap;
  uint8_t lut[256];
  QPoint dragbase;
  Qt::MouseButton dragbutton;
  Qt::KeyboardModifiers dragmods;
};

#endif
