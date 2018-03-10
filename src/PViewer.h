// PViewer.h

#ifndef PVIEWER_H

#define PVIEWER_H

#include <QLabel>
#include <QVector>

class PViewer: public QWidget {
public:
  PViewer(class Voxmap *vm, class IDmap *im, QWidget *parent=0);
  ~PViewer();
  void showTracings(int txt);
  void showOverlay(int txt);
  virtual void keyPressEvent(QKeyEvent *) override;
private:
  static int xDirForZAxis(int ax);
  static int yDirForZAxis(int ax);
  void showDecorated(QImage &img, int zax,
                     int zdir, int xdir, int ydir);
private:
  QVector<QLabel *> panels;
  class QGridLayout *grid;
  class Projection *proj;
  class Voxmap *vm;
  class IDmap *im;
};

#endif
