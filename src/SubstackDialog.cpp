// SubstackDialog.cpp

#include "SubstackDialog.h"
#include "ui_SubstackDialog.h"
#include <QAbstractButton>
#include <QMessageBox>
#include "IDmap.h"
#include <QPainter>
#include "DistinctColors.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDataStream>

inline int sign(float x) {
  return x<0 ? -1 : x>0 ? 1 : 0;
}

constexpr float THR = 9;

class SDData {
public:
  Transform3 t0, t1;
  Viewer *viewer;
  Ui_SubstackDialog *ui;
  QImage plainimg;
  QMap<uint16_t, Point3> centerofmass;
  SubstackDialog *me;
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
    
    viewer->hideMessages(true);
    Viewer::View savedview = viewer->currentView();
    viewer->setView(Viewer::None);
    QVector<QImage> stack;
    Transform3 delta(t0.delta(t1));
    for (int n=0; n<steps; n++) {
      Transform3 t = t0;
      t.addPartial(delta, n/(steps-.99999));
      viewer->setTransform(t);
      QPixmap pm(viewer->size());
      viewer->render(&pm);
      stack << pm.toImage().convertToFormat(QImage::Format_Grayscale8);
    }
    viewer->setTransform(t1);
    viewer->hideMessages(false);
    viewer->setView(savedview);
    
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
    plainimg = res;
    centerofmass = viewer->currentIDmap()->centersOfErodedMass();
    static DistinctColors dc;
    Transform3 t0inv = t0.inverse();
    Transform3 t1inv = t1.inverse();
    res = res.convertToFormat(QImage::Format_RGB32);
    QPainter ptr(&res);
    int fac = viewer->hiDPI();
    for (auto it=centerofmass.begin(); it!=centerofmass.end(); ++it) {
      uint16_t k = it.key();
      Point3 v = it.value();
      v *= 2;
      Point3 v0 = t0inv.apply(v);
      Point3 v1 = t1inv.apply(v);
      if (sign(v0.z) != sign(v1.z)
          || abs(v0.z)<THR || abs(v1.z)<THR) {
        qDebug() << "point" << k << v0;
        QPen p(dc.color(k));
        p.setWidth(3*fac);
        ptr.setPen(p);
        double x = (v0.x+v1.x)*fac/2.;
        double y = (v0.y+v1.y)*fac/2.;
        ptr.drawPoint(x, y);
        ptr.drawText(x, y, QString::number(k));
      }
    }
    ui->preview->setPixmap(QPixmap::fromImage(res));
  }
  void startExport() {
    qDebug() << "start export";
    QString fn = QFileDialog::getSaveFileName(0, "Export to vscope xml",
                                              "",
                                              "*.xml");
    if (fn.isEmpty()) {
      qDebug() << "no filename";
      return;
    }
    if (fn.endsWith(".xml"))
      fn = fn.left(fn.length() - 4);
    if (saveROIs(fn)
        && saveImage(fn)
        && saveCCDImage(fn)
        && saveBase(fn)) {
      me->hide();
    } else {
      QMessageBox::warning(0, "Substack exporter",
                           "Could not save " + fn);
    }
  }
  bool saveBase(QString basefn) {
    QFile f(basefn + ".xml");
    if (!f.open(QFile::WriteOnly))
      return false;
    QFile f0(":dummy.xml");
    f0.open(QFile::ReadOnly);
    f.write(f0.readAll());
    return true;
  }
  bool saveImage(QString basefn) {
    return plainimg.save(basefn + ".jpg");
  }
  void writeEmpty(QDataStream &ds) {
    for (int y=0; y<512; y++) {
      for (int x=0; x<512; x++) {
        uint16_t v = 0;
        ds << v;
      }
    }
  }
  bool saveCCDImage(QString basefn) {
    QImage scl = plainimg.scaled(QSize(512,512));
    QFile f(basefn + "-ccd.dat");
    if (!f.open(QFile::WriteOnly))
      return false;
    QDataStream ds(&f);
    for (int k=0; k<2; k++) {
      for (int y=511; y>=0; y--) {
        uint8_t const *src = scl.scanLine(y);
        for (int x=0; x<512; x++) {
          uint16_t v = src[x];
          v *= 255;
          ds << v;
        }
      }
    }
    return true;
  }
  bool saveROIs(QString basefn) {
    QFile f(basefn + "-rois.xml");
    if (!f.open(QFile::WriteOnly))
      return false;
    QTextStream ts(&f);
    ts << "<!DOCTYPE vsdscopeROIs>\n";
    ts << "<vsdscopeROIs>\n";
    ts << " <rois>\n";
    double xfac = 512.0 / plainimg.width();
    double yfac = 512.0 / plainimg.width();
    Transform3 t0inv = t0.inverse();
    Transform3 t1inv = t1.inverse();
    int fac = viewer->hiDPI();
    for (auto it=centerofmass.begin(); it!=centerofmass.end(); ++it) {
      uint16_t k = it.key();
      Point3 v = it.value();
      v *= 2;
      Point3 v0 = t0inv.apply(v);
      Point3 v1 = t1inv.apply(v);
      if (sign(v0.z) != sign(v1.z) || abs(v0.z)<THR || abs(v1.z)<THR) {
        double x = (v0.x+v1.x)*fac/2. * xfac;
        double y = (v0.y+v1.y)*fac/2. * yfac;
        ts << "<roi n=\"8\" cam=\"Top\" id=\"";
        ts << QString::number(k);
        ts << "\">";
        for (int q=0; q<8; q++) {
          double cs = cos(2*3.14*q/8);
          double sn = sin(2*3.14*q/8);
          ts << QString::number(x + 5*cs) << " "
             << QString::number(y + 5*sn) << "\n";
        }
        ts << "</roi>\n";
      }
    }
    ts << " </rois>\n";
    ts << "</vsdscopeROIs>\n";
    return true;
  }
};

SubstackDialog::SubstackDialog(Viewer *viewer, QWidget *parent):
  QDialog(parent), d(new SDData) {
  d->viewer = viewer;
  d->me = this;
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
  
