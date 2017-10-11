// PViewer.cpp

#include "PViewer.h"
#include <QGridLayout>
#include "Projection.h"

PViewer::PViewer(Voxmap *vm, IDmap *im, QWidget *parent):
  QWidget(parent), proj(new Projection(vm, im)) {
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
  if (txt=="1") {
    img = proj->flatids(0, 1);
    ttl = "Right";
  } else if (txt=="2") {
    img = proj->flatids(1, 1);
    ttl = "Ventral";
  } else if (txt=="3") {
    img = proj->flatids(2, 1);
    ttl = "Anterior";
  } else if (txt=="4") {
    img = proj->flatids(0, -1);
    ttl = "Left";
  } else if (txt=="5") {
    img = proj->flatids(1, -1);
    ttl = "Dorsal";
  } else if (txt=="6") {
    img = proj->flatids(2, -1);
    ttl = "Posterior";
  } else {
    img = proj->flatids(1, 1);
    ttl = "Ventral";
  }
  QPixmap pm(QPixmap::fromImage(img));
  panels[0]->setPixmap(pm);
  //  resize(pm.size()*2);
  setWindowTitle(ttl);
}
