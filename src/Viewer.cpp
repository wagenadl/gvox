// Viewer.cpp

#include "Viewer.h"
#include "Voxmap.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <math.h>
#include <QTime>

Viewer::Viewer(QWidget *parent): QLabel(parent) {
  voxmap = 0;
  hidpi_ = 3;
  setScaledContents(false);
  setMouseTracking(true);
  dragbutton = Qt::NoButton;

  for (int x=0; x<256; x++) {
    double y = x/255.0;
    y = pow((y-.1)/.8, .75);
    if (y<0)
      y = 0;
    else if (y>1)
      y = 1;
    lut[x] = int(255.99*y);
  }
}

void Viewer::setVoxmap(Voxmap *vm) {
  voxmap = vm;
  t = Transform3();
  if (voxmap) {
    t.shift(vm->width()/2, vm->height()/2, vm->depth()/2);
    t.shift(-width()/2./hidpi_, -height()/2./hidpi_, 0);
  }
  rebuild();
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {
  if (dragbutton==Qt::LeftButton && dragmods==Qt::NoModifier) {
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.shift(-delta.x()*1./hidpi_, -delta.y()*1./hidpi_, 0);
    rebuild();
  } else if (dragbutton==Qt::LeftButton && dragmods & Qt::ControlModifier) {
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.rotate(-delta.x()/200./hidpi_, -delta.y()/200./hidpi_,
	     dragbase.x()*1./hidpi_, dragbase.y()*1./hidpi_);
    rebuild();
  } else if (dragbutton==Qt::LeftButton && dragmods & Qt::ShiftModifier) {
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.rotatez(-delta.x()/200./hidpi_,
	      dragbase.x()*1./hidpi_, dragbase.y()*1./hidpi_);
    t.scale(exp(-delta.y()/200./hidpi_),
            dragbase.x()*1./hidpi_, dragbase.y()*1./hidpi_);            
    rebuild();
  }
}

void Viewer::mousePressEvent(QMouseEvent *e) {
  t0 = t;
  dragbase = e->pos();
  dragbutton = e->button();
  dragmods = e->modifiers();
}

void Viewer::mouseReleaseEvent(QMouseEvent *) {
  dragbutton = Qt::NoButton;
}

void Viewer::wheelEvent(QWheelEvent *e) {
  QPoint p = e->angleDelta();
  qDebug() << "wheel" << p;
  if (e->modifiers() & Qt::ShiftModifier) {
    t.scale(exp(-p.y()/200./hidpi_),
            dragbase.x()*1./hidpi_, dragbase.y()*1./hidpi_);        
  } else {
    t.shift(0, 0, p.y()/40.0);
  }
  rebuild();
}

void Viewer::resizeEvent(QResizeEvent *) {
  rebuild();
}

void Viewer::rebuild() {
  if (voxmap) {
    qDebug() << "rebuild";
    QTime time; time.start();
    int w = width() / hidpi_;
    int h = height() / hidpi_;
    QImage img(w, h, QImage::Format_Grayscale8);
    for (int y=0; y<h; y++) {
      uint8_t *bits = img.scanLine(y);
      voxmap->scanLineTril(t, y, 0, w, bits, lut);
    }
    qDebug() << time.elapsed();
    setPixmap(QPixmap::fromImage(img.scaled(hidpi_*w, hidpi_*h)));
  } else {
    setPixmap(QPixmap());
  }
}
