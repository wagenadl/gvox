// Viewer.cpp

#include "Viewer.h"
#include "Voxmap.h"
#include "IDmap.h"
#include "IDFactor.h"
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
#include <QTimerEvent>

Viewer::Viewer(QWidget *parent): QLabel(parent) {
  thickmod = 1;
  voxmap = 0;
  idmap = 0;
  view = All;
  idfactor = 1;
  hidpi_ = 2;
  setScaledContents(false);
  setMouseTracking(true);
  setFocusPolicy(Qt::WheelFocus);
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  dragbutton = Qt::NoButton;
  message = new QLabel(this);
  message2 = new QLabel(this);
  message->setPalette(QPalette(QColor("white"), QColor("black")));
  message2->setPalette(QPalette(QColor("white"), QColor("black")));
  message->setText("Initializing");
  message2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  setMode(Select);
  showcross = false;
  buildLUT();
}

Viewer::~Viewer() {
  for (auto p: pviewer)
    delete p;
}

inline uint8_t clip8(double z) {
  if (z<0)
    return 0;
  else if (z>1)
    return 255;
  else
    return uint8_t(255.99*z);
}

void Viewer::buildLUT(uint8_t blk, uint8_t wht, float gamma) {
  for (int x=0; x<256; x++) {
    double y = x/255.0;
    y = (y-(blk/255.0))/(wht/255.0 - blk/255.0);
    if (y<=0)
      y = 0;
    else if (y>=1)
      y = 1;
    else
      y = pow(y, gamma);
    for (int iz=0; iz<=HALFNZ*2; iz++) {
      double z = (iz - HALFNZ) * 1.0 / HALFNZ;
      double z1 = (z>0) ? z : 0;
      double z2 = (z<0) ? z : 0;
#if 1
      // green when in front of object, purple when behind it
      uint8_t g(clip8(y*(1-z1*.75)));
      uint8_t r(clip8(y*(1-.85*z2*z2)));
      uint8_t b(clip8(y*(1+.2*z2)));
#else
	 // blue when in front, yellow when beyind
	 uint8_t g(255.99*y*(1 + .5*z2));
	 uint8_t r(255.99*y*(1 + .75*z2));
	 uint8_t b(255.99*y*(1 - z1));
#endif
      lut[iz*256+x] = 0xff000000u + b + 256u*g + 65536u*r;
    }
  }
  rebuild();
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
  setMode(Select);
  rebuildID();
}

void Viewer::showOverlay(int k) {
  ensurePViewer(k);
  QCursor c0 = cursor();
  setCursor(Qt::WaitCursor);
  pviewer[k]->showOverlay(k);
  setCursor(c0);
}

void Viewer::showEOverlay(int k, QString name) {
  ensurePViewer(k+30);
  QCursor c0 = cursor();
  setCursor(Qt::WaitCursor);
  bool ok;
  int id = name.toInt(&ok);
  if (!ok)
    id = voxmap->find(name);
  if (id>0)
    pviewer[k+30]->showOverlay(k, id);
  else
    qDebug() << "No object “" << name << "” found";
  setCursor(c0);
}

void Viewer::showProjection(int k) {
  ensurePViewer(20+k);
  QCursor c0 = cursor();
  setCursor(Qt::WaitCursor);
  pviewer[20+k]->showProjection(k);
  setCursor(c0);
}

void Viewer::showTraces(int k) {
  ensurePViewer(10 + k);
  pviewer[10+k]->showTracings(k);
}

void Viewer::copy() {
  QGuiApplication::clipboard()->setPixmap(*pixmap());
}

void Viewer::add() {
  if (idmap) {
    paintid = idmap->max()+1;
    message->setText(QString("New ID #%1").arg(paintid));
    emit selectionChanged(paintid);
  }      
}

void Viewer::del() {
  if (idmap) {
    idmap->drop(paintid);
    message->setText(QString("Deleted ID #%1").arg(paintid));
    paintid = 0;
    rebuildID();
    emit selectionChanged(paintid);
  }
}

void Viewer::setView(View v) {
  view = v;
  rebuildID();
}

void Viewer::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_Slash:
    //toggleIDs();
    break;
  case Qt::Key_T:
    showTraces(lastkey.toInt());
    break;
  case Qt::Key_O:
    showOverlay(lastkey.toInt());
    break;
  case Qt::Key_E:
    doExport();
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
    copy();
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
  if (voxmap) {
    p = voxmap->pixtoum(p);
    message2->setText(QString("(%1,%2,%3 μm)")
               .arg(p.x, 7, 'f', 2).arg(p.y, 7, 'f', 2).arg(p.z, 7, 'f', 2));
  } else {
    message2->setText(QString("(%1,%2,%3)")
                      .arg(int(p.x)).arg(int(p.y)).arg(int(p.z)));
  }
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {
  qDebug() << "mousemove" << dragbutton << e->buttons();
  if (dragbutton==Qt::LeftButton && dragmods & Qt::ControlModifier) {
    // control-drag: rotate 3D
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.rotate(-delta.x()/200./hidpi_, -delta.y()/200./hidpi_,
	     dragbase.x()*1./hidpi_, dragbase.y()*1./hidpi_);
    showPos(e->pos());
    rebuild();
  } else if (dragbutton==Qt::LeftButton && (dragmods & Qt::ShiftModifier
				     || mode==Select)) {
    // shift-drag: move
    QPoint delta = e->pos() - dragbase;
    t = t0;
    t.shift(-delta.x()*1./hidpi_, -delta.y()*1./hidpi_, 0);
    showPos(e->pos());
    rebuild();
  } else if (dragbutton==Qt::LeftButton && dragmods==Qt::NoModifier
             && mode==Draw) {
    // paint!
    if (idmap) {
      Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
				e->pos().y()*1./hidpi_, 0)));
      if (paintid>0) {
        idmap->paint(p.x, p.y, p.z, paintid);
	message->setText(QString("Painting ID #%1").arg(paintid));
      }
      rebuildID();
    }
  } else if (dragbutton==Qt::LeftButton && dragmods==Qt::NoModifier
             && mode==Erase) {
    // erase    
    if (idmap) {
      Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
				e->pos().y()*1./hidpi_, 0)));
      for (int dx=-1; dx<=1; dx++)
        for (int dy=-1; dy<=1; dy++)
          for (int dz=-1; dz<=1; dz++)
            idmap->paint(p.x+dx, p.y+dy, p.z+dz, 0);
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
    if (mode==Draw && paintid>0) {
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
      message->setText(QString("Ball drawn for ID #%1").arg(paintid));
    } else if (mode==Erase) {
      const double Rf = 9./idfactor;
      const int R = ceil(Rf);
      Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
                                e->pos().y()*1./hidpi_, 0)));
      for (int dx=-R; dx<=R; dx++) 
        for (int dy=-R; dy<=R; dy++) 
          for (int dz=-R; dz<=R; dz++) 
            if (dx*dx + dy*dy + dz*dz < Rf*Rf)
              idmap->paint(p.x+dx, p.y+dy, p.z+dz, 0);
      message->setText(QString("Ball erased"));
    }
    rebuildID();
  } else if (e->button()==Qt::RightButton) {
    Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
                              e->pos().y()*1./hidpi_, 0)));
    paintid = idmap->getf(p.x, p.y, p.z);
    message->setText(QString("Selected %1").arg(paintid));
    emit selectionChanged(paintid);
  }
}

void Viewer::resetRotation() {
  if (voxmap) {
    Point3 p0 = t.apply(Point3(width()/2./hidpi_, height()/2./hidpi_));
    double d0 = pow(t.det(), 1/3.);
    t = voxmap->pixtoumTransform();
    double d1 = pow(t.det(), 1/3.);
    t.scale(d0/d1, 0, 0);
    Point3 p1 = t.apply(Point3(width()/2./hidpi_, height()/2./hidpi_));
    t = Transform3::shifter(p0.x-p1.x, p0.y-p1.y, p0.z-p1.z) * t;
    rebuild();
  }    
}

void Viewer::gotoXYZum(double x, double y, double z) {
  Point3 p1 = t.apply(Point3(width()/2./hidpi_, height()/2./hidpi_));
  Point3 p0(x, y, z);
  if (voxmap)
    p0 = voxmap->umtopix(p0);
  t = Transform3::shifter(p0.x-p1.x, p0.y-p1.y, p0.z-p1.z) * t;
  rebuild();
}

void Viewer::mousePressEvent(QMouseEvent *e) {
  t0 = t;
  dragbase = e->pos();
  dragbutton = e->button();
  qDebug() << "mousepress" << dragbutton;
  dragmods = e->modifiers();
  if (dragbutton==Qt::RightButton || mode==Select) {
    Point3 p(tid.apply(Point3(e->pos().x()*1./hidpi_,
			      e->pos().y()*1./hidpi_, 0)));
    paintid = idmap->getf(p.x, p.y, p.z);
    emit selectionChanged(paintid);
  } else if (dragmods==Qt::NoModifier) {
    mouseMoveEvent(e);
  }
}

void Viewer::mouseReleaseEvent(QMouseEvent *) {
  dragbutton = Qt::NoButton;
  qDebug() << "mouserelease";
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
    double scl = 1e2/((p1-p2).length());
    thickmod = scl > 1 ? scl : 1;
    message->setText(QString("Scale: %1%").arg(int(100*scl)));
    
  } else if (e->modifiers() & Qt::ShiftModifier
	     || mode==Select) {
    // wheel + shift: move in Z
    t.shift(0, 0, delta.y()/40.0);
  } 
  showPos(e->pos());
  rebuild();
}

void Viewer::resizeEvent(QResizeEvent *e) {
  QSize o = e->oldSize();
  QSize n = e->size();
  Point3 p0(t.apply(Point3(o.width()/2./hidpi_, o.height()/2./hidpi_,0)));
  rebuild();
  Point3 p1(t.apply(Point3(n.width()/2./hidpi_, n.height()/2./hidpi_,0)));
  t = Transform3::shifter(p0.x-p1.x, p0.y-p1.y, p0.z-p1.z) * t;
  
  message->move(5, height() - 50);
  message->resize(width()-10, 35);
  message2->move(5, height() - 50);
  message2->resize(width()-10, 35);
}

void Viewer::gotoCenter() {
  if (voxmap) {
    t = Transform3();
    t.m[0][3] = voxmap->width()/2 - width()/2./hidpi_;
    t.m[1][3] = voxmap->height()/2 - height()/2./hidpi_;
    t.m[2][3] = voxmap->depth()/2;
  } else {
    qDebug() << "Cannot center - no voxmap";
  }
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
	  voxmap->scanLineTrilDepth(t, y, w, HALFNZ*2+1, bits, lut);
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

static constexpr int THICKNESS = 2;

void Viewer::rebuildID() {
  static DistinctColors dc;
  int w = width() / hidpi_;
  int h = height() / hidpi_;
  if (idmap && view!=None) {
    QImage img(im0);
    img.setPixel(QPoint(0,0), 0xffffffff); // force detach
    int nthreads = 4;
    std::thread *thr[nthreads];
    for (int i=0; i<nthreads; i++) {
      int y0 = h*i/nthreads;
      int y1 = h*(i+1)/nthreads;
      auto foo = [&](int y0, int y1) {
        QMap<int, bool> use;
	uint16_t buf[w];
	uint16_t buf2[w];
	for (int y=y0; y<y1; y++) {
	  uint32_t *bits = (uint32_t*)(img.scanLine(y));
	  idmap->thickScanLine(tid, y, w, THICKNESS*thickmod, buf, buf2);
	  for (int x=0; x<w; x++) {
            int v = buf[x];
            if (v && !use.contains(v)) {
              switch (view) {
              case All:
                use[v] = true;
                break;
              case Named:
                use[v] = voxmap->name(v) != "";
                break;
              case Anon:
                use[v] = voxmap->name(v) == "";
                break;
              case None:
                use[v] = false;
              }
            }
	    if (v && use[v]) {
	      uint8_t *dst = (uint8_t*)(bits);
	      uint32_t cc = dc.color(v);
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

  if (showcross) {
    QPainter ptr;
    ptr.begin(this);
    ptr.setPen(QPen(QColor(255,255,255), 2));
    ptr.drawLine(width()/2-20, height()/2,
                 width()/2+20, height()/2);
    ptr.drawLine(width()/2, height()/2-20,
                 width()/2, height()/2+20);
    ptr.end();
  }
}

void Viewer::ensurePViewer(int k) {
  if (!pviewer.contains(k)) 
    pviewer[k] = new PViewer(voxmap, idmap);
  pviewer[k]->show();
}

void Viewer::find(int id) {
  gotoID(id);
}

bool Viewer::find(QString name) {
  qDebug() << "find" << name;
  int id = voxmap->find(name);
  qDebug() << "-> id" << id;
  if (id>0)
    gotoID(id);
  return id>0;
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
  emit selectionChanged(paintid);
  message->setText(QString("Centered on ID #%1").arg(id));
  showcross = true;
  startTimer(1000);
}

void Viewer::timerEvent(QTimerEvent *e) {
  killTimer(e->timerId());
  showcross = false;
  update();
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

void Viewer::setMode(Viewer::Mode m) {
  mode = m;
}

void Viewer::setName(QString name) {
  if (paintid>0) {
    voxmap->setName(paintid, name);
    if (view==Anon || view == Named)
      rebuildID();
  }
}

void Viewer::doExport() {
  if (idmap && voxmap) {
    QString ofn = voxmap->basename() + ".txt";
    if (idmap->textExport(ofn, idfactor))
      message->setText("Exported to " + ofn);
  }
}  

