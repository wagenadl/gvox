// Viewer.h

#ifndef VIEWER_H

#define VIEWER_H

#include "Transform3.h"
#include <QLabel>

class Viewer: public QLabel {
public:
  Viewer(QWidget *parent=0);
  void setVoxmap(class Voxmap *voxmap);
  virtual void mouseMoveEvent(QMouseEvent *) override;
  virtual void mousePressEvent(QMouseEvent *) override;
  virtual void mouseReleaseEvent(QMouseEvent *) override;
  virtual void wheelEvent(QWheelEvent *) override;
  virtual void resizeEvent(QResizeEvent *) override;
  virtual void paintEvent(QPaintEvent *) override;
protected:
  void rebuild();
private:
  Transform3 t;
  int hidpi_;
  Voxmap *voxmap;
  uint8_t lut[256];
  QPoint dragbase;
  Transform3 t0;
  Qt::MouseButton dragbutton;
  Qt::KeyboardModifiers dragmods;
};

#endif
