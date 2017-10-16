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
#include "PViewer.h"
#include <QGuiApplication>
#include <QClipboard>

Viewer::Viewer(QWidget *parent): QLabel(parent) {
  pviewer = 0;
  voxmap = 0;
  idmap = 0;
  showids = true;
  idfactor = 1;
  hidpi_ = 3;
  setScaledContents(false);
  setMouseTracking(true);
  setFocusPolicy(Qt::WheelFocus);
  dragbutton = Qt::NoButton;
  buildLUT();
  message = new QLabel(this);
  message2 = new QLabel(this);
  message->setPalette(QPalette(QColor("white"), QColor("black")));
  message2->setPalette(QPalette(QColor("white"), QColor("black")));
  message->setText("Initializing");
  message2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  setViewMode();
}

Viewer::~Viewer() {
  delete pviewer;
}

void Viewer::buildLUT() {
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
#if 1
      // green when in front of object, purple when behind it
	 uint8_t g(255.99*y*(1-z1*.75));
	 uint8_t r(255.99*y*(1-.85*z2*z2));
	 uint8_t b(255.99*y*(1+.2*z2));
#else
	 // blue when in front, yellow when beyind
	 uint8_t g(255.99*y*(1 + .5*z2));
	 uint8_t r(255.99*y*(1 + .75*z2));
	 uint8_t b(255.99*y*(1 - z1));
#endif
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
  message->setText(QString("Voxmap: %1 x %2 x %3")
		   .arg(vm->width()).arg(vm->height()).arg(vm->depth()));
}

void Viewer::setIDmap(IDmap *im, int f) {
  idmap = im;
  idfactor = f;
  setViewMode();
  rebuildID();
}

void Viewer::setViewMode() {
  paintid = VIEWID;
  message->setText(QString("View mode"));
}

void Viewer::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_Delete: case Qt::Key_Backspace:
    paintid = DELETEID;
    message->setText(QString("Erasing"));
    break;
  case Qt::Key_Space:
    setViewMode();
    break;
  case Qt::Key_Slash:
    showids = !showids;
    rebuildID();
    break;
  case Qt::Key_Enter: case Qt::Key_Return:
    if (idmap) {
      paintid = idmap->max()+1;
      message->setText(QString("New ID #%1").arg(paintid));
    } else {
      setViewMode();
    }      
    break;
  case Qt::Key_T:
    ensurePViewer();
    pviewer->showTracings(lastkey);
    break;
  case Qt::Key_O: {
    ensurePViewer();
    QCursor c0 = cursor();
    setCursor(Qt::WaitCursor);
    pviewer->showOverlay(lastkey);
    setCursor(c0);
  } break;
  case Qt::Key_E:
    if (idmap && voxmap) {
      QString ofn = voxmap->basename() + ".txt";
      if (idmap->textExport(ofn, idfactor))
	message->setText("Exported to " + ofn);
    }
    break;
  case Qt::Key_G:
    gotoID(lastkey.toInt());
    break;
  case Qt::Key_D:
    if (idmap) {
      int id = lastkey.toInt();
      if (id) {
	idmap->drop(id);
	message->setText(QString("Deleted ID #%1").arg(id));
	rebuildID();
      }
    }
    break;
  case Qt::Key_C:
    QGuiApplication::clipboard()->setPixmap(*pixmap());
    break;
  }
  if (e->text()>="0" && e->text()<="9")
    lastkey += e->text();
  else
    lastkey = "";
}

void Viewer::showPos(QPoint p) {
  showPos(t.apply(Point3(p.x()*1./hidpi_, p.y()*1./hidpi_, 0)));
}

void Viewer::showPos(Point3 p) {
  message2->setText(QString("(%1,%2,%3)")
                      .arg(int(p.x)).arg(int(p.y)).arg(int(p.z)));
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {
  if (dragbutton==Qt::LeftButton && dragmods & Qt::ShiftModifier) {
    // shift-drag: move
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.shift(-delta.x()*1./hidpi_, -delta.y()*1./hidpi_, 0);
    showPos(e->pos());
    rebuild();
  } else if (dragbutton==Qt::LeftButton && dragmods & Qt::ControlModifier) {
    // control-drag: rotate 3D
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.rotate(-delta.x()/200./hidpi_, -delta.y()/200./hidpi_,
	     dragbase.x()*1./hidpi_, dragbase.y()*1./hidpi_);
    showPos(e->pos());
    rebuild();
  } else if (dragbutton==Qt::LeftButton && dragmods==Qt::NoModifier) {
    // paint!
    if (idmap) {
      Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
				e->pos().y()*1./hidpi_, 0)));
      if (paintid==DELETEID) {
        for (int dx=-1; dx<=1; dx++)
          for (int dy=-1; dy<=1; dy++)
            for (int dz=-1; dz<=1; dz++)
              idmap->paint(p.x+dx, p.y+dy, p.z+dz, paintid);
      } else if (paintid>0) {
        idmap->paint(p.x, p.y, p.z, paintid);
	message->setText(QString("Painting ID #%1").arg(paintid));
      }
      rebuildID();
    }
  } else {
    showPos(e->pos());
  }
}

void Viewer::mouseDoubleClickEvent(QMouseEvent *e) {
  if (!idmap)
    return;
  if (e->button()==Qt::LeftButton && e->modifiers()==Qt::NoModifier) {
    if (paintid != VIEWID) {
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
      if (paintid==DELETEID)
        message->setText(QString("Ball for deletion"));
      else
        message->setText(QString("Ball for ID #%1").arg(paintid));
      rebuildID();
    }
  } else if (e->button()==Qt::RightButton) {
      Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
				e->pos().y()*1./hidpi_, 0)));
      paintid = idmap->getf(p.x, p.y, p.z);
      message->setText(QString("Adding to ID #%1").arg(paintid));
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
    message->setText(QString("Adding to ID #%1").arg(paintid));
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
    Point3 p1(t.apply(Point3(0,0,0)));
    Point3 p2(t.apply(Point3(100,0,0)));
    message->setText(QString("Scale: %1%").arg(int(1e4/((p1-p2).length()))));
  } else if (e->modifiers() & Qt::ShiftModifier) {
    // wheel + shift: move in Z
    t.shift(0, 0, delta.y()/40.0);
  } 
  showPos(e->pos());
  rebuild();
}

void Viewer::resizeEvent(QResizeEvent *) {
  rebuild();
  message->move(5, height() - 40);
  message->resize(width()-10, 35);
  message2->move(width()*2/3, height() - 40);
  message2->resize(width()*1/3 -5, 35);
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
  if (idmap && showids) {
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

    drawAxis("x", QColor(255,0,0), dx3);
    drawAxis("y", QColor(0,180,0), dy3);
    drawAxis("z", QColor(0,0,255), dz3);
  }
}

void Viewer::ensurePViewer() {
  if (!pviewer) 
    pviewer = new PViewer(voxmap, idmap);
  pviewer->show();
}

void Viewer::gotoID(int id) {
  if (!id)
    return;
  QVector<iPoint3> pp = idmap->extract(id);
  if (pp.isEmpty())
    return;
  Point3 sumx;
  int N = pp.length();
  for (iPoint3 const &p: pp) {
    sumx.x += p.x;
    sumx.y += p.y;
    sumx.z += p.z;
  }
  Point3 cm = sumx;
  cm /= N;
  Point3 current = t.apply(Point3(width()/2/hidpi_, height()/2/hidpi_, 0));
  cm *= idfactor;
  // not bothering with subpixel precision
  Point3 delta = cm - current;
  t = Transform3::shifter(delta.x, delta.y, delta.z) * t;
  rebuild();
  showPos(cm);
  paintid = id;
  message->setText(QString("Centered on ID #%1").arg(id));
}

QString Viewer::axlabel(QString ax) {
  if (voxmap)
    return voxmap->label(ax);
  else
    return ax;
}

void Viewer::drawAxis(QString ax, QColor c, Point3 v) {
  QPainter p(this);
  QPointF scr(v.x, v.y);
  float S = 30*hidpi_;
  QPointF XY0(S*2, S*2);
  QColor czp = QColor(255,192,255);
  QColor czn = QColor(160,255,170);
  QColor cp = c;
  QColor cn = c;
  if (fabs(v.z) > .7*v.length()) {
    if (v.z>0) {
      cp = czp;
      cn = czn;
    } else {
      cp = czn;
      cn = czp;
    }
  }
  p.setPen(QPen(cp, 2));
  p.drawLine(XY0, XY0 + .8*scr*S);
  p.drawText(QRectF(XY0+scr*S, QSize(100,100)).translated(-50, -50),
             Qt::AlignCenter, axlabel(ax+"p"));
  p.setPen(QPen(cn, 2));
  p.drawLine(XY0, XY0 - .8*scr*S);
  p.drawText(QRectF(XY0-scr*S, QSize(100,100)).translated(-50, -50),
             Qt::AlignCenter, axlabel(ax+"n"));
}   
