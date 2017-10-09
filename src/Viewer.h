// Viewer.h

#ifndef VIEWER_H

#define VIEWER_H

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
  float x0, y0, z0;
  float dxx, dxy, dxz;
  float dyx, dyy, dyz;
  int hidpi_;
  Voxmap *voxmap;
  uint8_t lut[256];
  QPoint dragbase;
  Qt::MouseButton dragbutton;
  Qt::KeyboardModifiers dragmods;
};

#endif
