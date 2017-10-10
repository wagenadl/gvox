// Viewer.cpp

#include "Viewer.h"
#include "Voxmap.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <math.h>
#include <QTime>
#include <thread>
#include <QPainter>

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
    int nthreads = 4;
    std::thread *thr[nthreads];
    for (int i=0; i<nthreads; i++) {
      int y0 = h*i/nthreads;
      int y1 = h*(i+1)/nthreads;
      auto foo = [&](int y0, int y1) {
	for (int y=y0; y<y1; y++) {
	  uint8_t *bits = img.scanLine(y);
	  voxmap->scanLineTril(t, y, 0, w, bits, lut);
	  voxmap->scanLineTrilDepth(t, y, w, 10, bits, lut);
	}
      };
      thr[i] = new std::thread{foo, y0, y1};
    }
    for (int i=0; i<nthreads; i++) {
      thr[i]->join();
    }
    for (int i=0; i<nthreads; i++) {
      delete thr[i];
    }
    qDebug() << time.elapsed();
    setPixmap(QPixmap::fromImage(img.scaled(hidpi_*w, hidpi_*h)));
  } else {
    setPixmap(QPixmap());
  }
}

inline float sq(float x) {
  return x*x;
}

inline float len(QPointF x) {
  return sqrt(sq(x.x()) + sq(x.y()));
}

void Viewer::paintEvent(QPaintEvent *e) {
  QLabel::paintEvent(e);
  if (voxmap) {
    Transform3 tinv = t.inverse();
    int X = voxmap->width();
    int Y = voxmap->height();
    int Z = voxmap->depth();
    Point3 xplus = tinv.apply(Point3(X, Y/2., Z/2.));
    Point3 xmin = tinv.apply(Point3(0, Y/2., Z/2.));
    Point3 yplus = tinv.apply(Point3(X/2., Y, Z/2.));
    Point3 ymin = tinv.apply(Point3(X/2., 0, Z/2.));
    Point3 zplus = tinv.apply(Point3(X/2., Y/2., Z));
    Point3 zmin = tinv.apply(Point3(X/2., Y/2., 0));
    Point3 dx3 = xplus - xmin;
    Point3 dy3 = yplus - ymin;
    Point3 dz3 = zplus - zmin;
    dx3 /= dx3.length() + 1e-9;
    dy3 /= dy3.length() + 1e-9;
    dz3 /= dz3.length() + 1e-9;

    QPainter p(this);
    QPointF dx(dx3.x, dx3.y);
    QPointF dy(dy3.x, dy3.y);
    QPointF dz(dz3.x, dz3.y);
    p.setPen(QPen("red"));
    float S = 50*hidpi_;
    QPointF XY0(S, S);
    p.drawLine(XY0, XY0 + dx*S);
    p.setPen(QPen("green"));
    p.drawLine(XY0, XY0 + dy*S);
    p.setPen(QPen("blue"));
    p.drawLine(XY0, XY0 + dz*S);
  }
}
