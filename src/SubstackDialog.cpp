// SubstackDialog.cpp

#include "SubstackDialog.h"
#include "ui_SubstackDialog.h"
#include <QAbstractButton>
#include <QMessageBox>

class SDData {
public:
  Transform3 t0, t1;
  Viewer *viewer;
  Ui_SubstackDialog *ui;
public:
  static double sq(double x) { return x*x; }
  bool checkLinear() {
    double e = 0;
    for (int k=0; k<3; k++)
      for (int l=0; l<3; l++)
        e += sq(t1.m[k][l] - t0.m[k][l]);
    return e < .0001;
  }
  void rebuild() {
    qDebug() << "SD rebuild";
    qDebug() << t0;
    qDebug() << t1;
    double dp = 0;
    for (int k=0; k<3; k++)
      dp += sq(t1.m[k][3] - t0.m[k][3]);
    dp = sqrt(dp);
    int steps = ceil(dp);
    if (steps==0)
      steps = 1;
    
    QSize s = viewer->size();
    viewer->hideMessages(true);
    QVector<QImage> stack;
    for (int n=0; n<steps; n++) {
      Transform3 t = t0;
      for (int k=0; k<3; k++)
        t.m[k][3] += (t1.m[k][3] - t0.m[k][3])*n/(steps-1);
      viewer->setTransform(t);
      QPixmap pm(viewer->size());
      viewer->render(&pm);
      stack << pm.toImage().convertToFormat(QImage::Format_Grayscale8);
    }
    viewer->setTransform(t1);
    viewer->hideMessages(false);
    
    QImage res = stack[0];
    int L = res.bytesPerLine() * res.height();
    unsigned char *dst = res.bits();
    QVector<unsigned char const *> srcs;
    for (int n=0; n<steps; n++) 
      srcs << stack[n].constBits();
    qDebug() << "steps" << steps << L;
    for (int n=0; n<steps; n++)
      qDebug() << "bits" << stack[n].bytesPerLine() * stack[n].height();
    for (int l=0; l<L; l++) {
      int v = 0;
      for (int n=0; n<steps; n++) {
        int v1 = srcs[n][l];
        if (v1>v)
          v = v1;
      }
      dst[l] = v;
    }
    ui->preview->setPixmap(QPixmap::fromImage(res));
  }
  void startExport() {
    qDebug() << "start export";
  }
};

SubstackDialog::SubstackDialog(Viewer *viewer, QWidget *parent):
  QDialog(parent), d(new SDData) {
  d->viewer = viewer;
  d->ui = new Ui_SubstackDialog();
  d->ui->setupUi(this);
  connect(d->ui->save, &QAbstractButton::clicked,
          [this]() { d->startExport(); });
}

SubstackDialog::~SubstackDialog() {
  delete d;
}

void SubstackDialog::activate(Transform3 base) {
  d->t0 = base;
  d->t1 = d->viewer->currentTransform();
  if (!d->checkLinear()) {
    QMessageBox::warning(0, "Substack exporter",
                         "Your marked location is not related"
                         " to current position by a linear shift");
    hide();
  } else {
    d->rebuild();
    show();
  }
}
  
