// Viewer.cpp

#include "Viewer.h"
#include "Voxmap.h"
#include "IDmap.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <math.h>
#include <QTime>
#include <thread>
#include <QPainter>
#include "DistinctColors.h"

Viewer::Viewer(QWidget *parent): QLabel(parent) {
  voxmap = 0;
  idmap = 0;
  idfactor = 1;
  hidpi_ = 3;
  setScaledContents(false);
  setMouseTracking(true);
  setFocusPolicy(Qt::WheelFocus);
  dragbutton = Qt::NoButton;

  for (int x=0; x<256; x++) {
    double y = x/255.0;
    y = pow((y-.1)/.75, .5);
    if (y<0)
      y = 0;
    else if (y>1)
      y = 1;
    for (int iz=0; iz<=HALFNZ*2; iz++) {
      double z = (iz - HALFNZ) * 1.0 / HALFNZ;
      double z1 = (z>0) ? z : 0;
      double z2 = (z<0) ? z : 0;
      uint8_t g(255.99*y*(1-z1*.5));
      uint8_t r(255.99*y*(1+z2*.5));
      uint8_t b(255.99*y);
      lut[iz*256+x] = 0xff000000 + b + 256*g + 65536*r;
    }
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

void Viewer::setIDmap(IDmap *im, int f) {
  idmap = im;
  idfactor = f;
  paintid = (im) ? im->max()+1 : 1;
  rebuildID();
}

void Viewer::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_Delete: case Qt::Key_Backspace:
    paintid = 0;
    qDebug() << "deleting";
    break;
  case Qt::Key_Enter: case Qt::Key_Return:
    qDebug() << "new";
      paintid = (idmap) ? idmap->max()+1 : 1;
    break;
  }
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {
  if (dragbutton==Qt::LeftButton && dragmods & Qt::ShiftModifier) {
    // shift-drag: move
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.shift(-delta.x()*1./hidpi_, -delta.y()*1./hidpi_, 0);
    rebuild();
  } else if (dragbutton==Qt::LeftButton && dragmods & Qt::ControlModifier) {
    // control-drag: rotate 3D
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.rotate(-delta.x()/200./hidpi_, -delta.y()/200./hidpi_,
	     dragbase.x()*1./hidpi_, dragbase.y()*1./hidpi_);
    rebuild();
  } else if (dragbutton==Qt::LeftButton && dragmods==Qt::NoModifier) {
    // paint!
    if (idmap) {
      Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
				e->pos().y()*1./hidpi_, 0)));
      if (paintid==0) {
        for (int dx=-1; dx<=1; dx++)
          for (int dy=-1; dy<=1; dy++)
            for (int dz=-1; dz<=1; dz++)
              idmap->paint(p.x+dx, p.y+dy, p.z+dz, paintid);
      } else {
        idmap->paint(p.x, p.y, p.z, paintid);
      }
      rebuildID();
    }
  }
}

void Viewer::mouseDoubleClickEvent(QMouseEvent *e) {
  if (!idmap)
    return;
  if (e->button()==Qt::LeftButton && e->modifiers()==Qt::NoModifier) {
    // paint cell body
    const double Rf = 9./idfactor;
    const int R = ceil(Rf);
    Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
			      e->pos().y()*1./hidpi_, 0)));
    for (int dx=-R; dx<=R; dx++) 
      for (int dy=-R; dy<=R; dy++) 
	for (int dz=-R; dz<=R; dz++) 
	  if (dx*dx + dy*dy + dz*dz < Rf*Rf)
	    idmap->paint(p.x+dx, p.y+dy, p.z+dz, paintid);
    rebuildID();
  } else if (e->button()==Qt::RightButton) {
      Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
				e->pos().y()*1./hidpi_, 0)));
      paintid = idmap->getf(p.x, p.y, p.z);
  }
}
    

void Viewer::mousePressEvent(QMouseEvent *e) {
  t0 = t;
  dragbase = e->pos();
  dragbutton = e->button();
  dragmods = e->modifiers();
  if (dragbutton==Qt::RightButton) {
    Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
			      e->pos().y()*1./hidpi_, 0)));
      paintid = idmap->getf(p.x, p.y, p.z);
  } else if (dragmods==Qt::NoModifier) {
    mouseMoveEvent(e); // treat as paint
  }
  
}

void Viewer::mouseReleaseEvent(QMouseEvent *) {
  dragbutton = Qt::NoButton;
}

void Viewer::wheelEvent(QWheelEvent *e) {
  QPoint delta = e->angleDelta();
  if (e->modifiers() & Qt::ControlModifier) {
    QPointF pos = e->pos(); pos /= hidpi_;
    // wheel + control: rotate in plane or scale
    //// t.rotatez(-delta.x()/200./hidpi_, pos.x(), pos.y());
    t.scale(exp(-delta.y()/200./hidpi_), pos.x(), pos.y());
    rebuild();
  } else if (e->modifiers() & Qt::ShiftModifier) {
    // wheel + shift: move in Z
    t.shift(0, 0, delta.y()/40.0);
  } 
  rebuild();
}

void Viewer::resizeEvent(QResizeEvent *) {
  rebuild();
}

void Viewer::rebuild() {
  tid = Transform3::scaler(1./idfactor) * t;
  if (voxmap) {
    int w = width() / hidpi_;
    int h = height() / hidpi_;
    QImage img(w, h, QImage::Format_RGB32);
    int nthreads = 4;
    std::thread *thr[nthreads];
    for (int i=0; i<nthreads; i++) {
      int y0 = h*i/nthreads;
      int y1 = h*(i+1)/nthreads;
      auto foo = [&](int y0, int y1) {
	for (int y=y0; y<y1; y++) {
	  uint32_t *bits = (uint32_t*)(img.scanLine(y));
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
    im0 = img;
    rebuildID();
  } else {
    setPixmap(QPixmap());
  }
}

void Viewer::rebuildID() {
  static DistinctColors dc;
  int w = width() / hidpi_;
  int h = height() / hidpi_;
  if (idmap) {
    QImage img(im0);
    img.setPixel(QPoint(0,0), 0xffffffff); // force detach
    int nthreads = 4;
    std::thread *thr[nthreads];
    for (int i=0; i<nthreads; i++) {
      int y0 = h*i/nthreads;
      int y1 = h*(i+1)/nthreads;
      auto foo = [&](int y0, int y1) {
	uint16_t buf[w];
	for (int y=y0; y<y1; y++) {
	  uint32_t *bits = (uint32_t*)(img.scanLine(y));
	  idmap->scanLine(tid, y, w, buf);
	  for (int x=0; x<w; x++) {
	    if (buf[x]) {
	      uint8_t *dst = (uint8_t*)(bits);
	      uint32_t cc = dc.color(buf[x]);
	      uint8_t const *src = (uint8_t const *)(&cc);
	      int r = dst[0];
	      int g = dst[1];
	      int b = dst[2];
	      r *= src[0];
	      g *= src[1];
	      b *= src[2];
	      dst[0] = r/256;
	      dst[1] = g/256;
	      dst[2] = b/256;
	    }
	    bits+=1;
	  }
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
    setPixmap(QPixmap::fromImage(img.scaled(hidpi_*w, hidpi_*h)));
  } else {
    setPixmap(QPixmap::fromImage(im0.scaled(hidpi_*w, hidpi_*h)));
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
