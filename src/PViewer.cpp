// PViewer.cpp

#include "PViewer.h"
#include <QGridLayout>
#include "Projection.h"
#include <QPainter>
#include "Voxmap.h"
#include <QKeyEvent>
#include <QGuiApplication>
#include <QClipboard>

PViewer::PViewer(Voxmap *vm, IDmap *im, QWidget *parent):
  QWidget(parent), proj(new Projection(vm, im)), vm(vm), im(im) {
  panels.append(new QLabel);
  grid = new QGridLayout;
  panels[0]->setScaledContents(true);
  grid->addWidget(panels[0], 0, 0);
  setLayout(grid);
  setFocusPolicy(Qt::WheelFocus);
}

PViewer::~PViewer() {
  delete proj;
}

int PViewer::xDirForZAxis(int ax) {
  switch (ax) {
  case 0: return -1;
  case 1: return -1;
  case 2: return 1;
  default: return 1;
  }
}

int PViewer::yDirForZAxis(int ax) {
  switch (ax) {
  case 0: return 1;
  case 1: return -1;
  case 2: return 1;
  default: return 1;
  }
}

void PViewer::showTracings(QString txt) {
  bool ok;
  int arg = txt.toInt(&ok) - 1;
  if (!ok || arg<0)
    arg = 1;
  int zax = arg%3;
  int zdir = (arg/3) ? -1 : 1;
  int xdir = xDirForZAxis(zax);
  int ydir = yDirForZAxis(zax);
  if (zdir<0)
    xdir = -xdir;
  QImage img = proj->flatids(zax, zdir, xdir, ydir);

  showDecorated(img, zax, zdir, xdir, ydir);
}

void PViewer::showDecorated(QImage &img, int zax,
                            int zdir, int xdir, int ydir) {
  int xax = (zax==0) ? 2 : 0;
  int yax = (zax==1) ? 2 : 1;
  QString zname = QChar('x' + zax);
  QString ttl = vm->label(zname + ((zdir>0) ? "positive" : "negative"));
  QString xname = QChar('x' + xax);
  QString yname = QChar('x' + yax);
  QString lbl = vm->label(xname + ((xdir<0) ? "positive" : "negative"))
    + " " + vm->label(yname + ((ydir<0) ? "positive" : "negative"));

  QPainter ptr(&img);
  ptr.setPen(QColor(255, 255, 255));
  ptr.drawText(5, 30, lbl);
  ptr.drawText(5, img.height() - 10, ttl + " aspect");
  QPixmap pm(QPixmap::fromImage(img));
  img.save(vm->basename() + "-" + ttl + "-traces.jpg");
  panels[0]->setPixmap(pm);
  setWindowTitle(ttl);
}

void PViewer::showOverlay(QString txt) {
  bool ok;
  int arg = txt.toInt(&ok) - 1;
  if (!ok || arg<0)
    arg = 1;
  int zax = arg%3;
  int zdir = (arg/3) ? -1 : 1;
  int xdir = xDirForZAxis(zax);
  int ydir = yDirForZAxis(zax);
  if (zdir<0)
    xdir = -xdir;
  QImage img = proj->overlay(zax, zdir, xdir, ydir);

  showDecorated(img, zax, zdir, xdir, ydir);
}

void PViewer::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_C:
    QGuiApplication::clipboard()->setPixmap(*(panels[0]->pixmap()));
    break;
  }
}
