// PViewer.cpp

#include "PViewer.h"
#include <QGridLayout>
#include "Projection.h"
#include <QPainter>
#include "Voxmap.h"

PViewer::PViewer(Voxmap *vm, IDmap *im, QWidget *parent):
  QWidget(parent), proj(new Projection(vm, im)), vm(vm), im(im) {
  panels.append(new QLabel);
  grid = new QGridLayout;
  panels[0]->setScaledContents(true);
  grid->addWidget(panels[0], 0, 0);
  setLayout(grid);
}

PViewer::~PViewer() {
  delete proj;
}

void PViewer::showTracings(QString txt) {
  QImage img;
  QString ttl;
  QString lbl;

  if (txt=="1") {
    img = proj->flatids(0, 1, -1, 1);
    ttl = "Right";
    lbl = "Ant. Dorsal";
  } else if (txt=="2") {
    img = proj->flatids(1, 1, -1, -1);
    ttl = "Ventral";
    lbl = "R. Ant.";
  } else if (txt=="3") {
    img = proj->flatids(2, 1, 1, 1);
    ttl = "Anterior";
    lbl = "L. Dorsal";
  } else if (txt=="4") {
    img = proj->flatids(0, -1, 1, 1);
    ttl = "Left";
    lbl = "Post. Dorsal";
  } else if (txt=="5") {
    img = proj->flatids(1, -1, 1, -1);
    ttl = "Dorsal";
    lbl = "L. Ant.";
  } else if (txt=="6") {
    img = proj->flatids(2, -1, -1, 1);
    ttl = "Posterior";
    lbl = "R. Dorsal";
  } else {
    img = proj->flatids(1, 1, -1, -1);
    ttl = "Ventral";
    lbl = "R. Ant.";
  }
  QPainter ptr(&img);
  ptr.setPen(QColor(255, 255, 255));
  ptr.drawText(5, 30, lbl);
  ptr.drawText(5, img.height() - 10, ttl + " aspect");
  QPixmap pm(QPixmap::fromImage(img));
  img.save(vm->basename() + "-" + ttl + "-traces.jpg");
  panels[0]->setPixmap(pm);
  //  resize(pm.size()*2);
  setWindowTitle(ttl);
}

void PViewer::showOverlay(QString txt) {
  QImage img;
  QString ttl;
  QString lbl;
  panels[0]->setPixmap(QPixmap());
  repaint();
  if (txt=="1") {
    img = proj->overlay(0, 1, -1, 1);
    ttl = "Right";
    lbl = "Ant. Dorsal";
  } else if (txt=="2") {
    img = proj->overlay(1, 1, -1, -1);
    ttl = "Ventral";
    lbl = "R. Ant.";
  } else if (txt=="3") {
    img = proj->overlay(2, 1, 1, 1);
    ttl = "Anterior";
    lbl = "L. Dorsal";
  } else if (txt=="4") {
    img = proj->overlay(0, -1, 1, 1);
    ttl = "Left";
    lbl = "Post. Dorsal";
  } else if (txt=="5") {
    img = proj->overlay(1, -1, -1, 1);
    ttl = "Dorsal";
    lbl = "L. Ant.";
  } else if (txt=="6") {
    img = proj->overlay(2, -1, -1, 1);
    ttl = "Posterior";
    lbl = "R. Dorsal";
  } else {
    img = proj->overlay(1, 1, -1, -1);
    ttl = "Ventral";
    lbl = "R. Ant.";
  }
  QPainter ptr(&img);
  ptr.setPen(QColor(255, 255, 255));
  ptr.drawText(5, 30, lbl);
  ptr.drawText(5, img.height()-10, ttl + " aspect");
  QPixmap pm(QPixmap::fromImage(img));
  img.save(vm->basename() + "-" + ttl + ".jpg");
  panels[0]->setPixmap(pm);
  //  resize(pm.size()*2);
  setWindowTitle(ttl);
}
