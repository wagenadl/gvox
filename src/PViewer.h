// PViewer.h

#ifndef PVIEWER_H

#define PVIEWER_H

#include <QLabel>
#include <QVector>

class PViewer: public QWidget {
public:
  PViewer(class Voxmap *vm, class IDmap *im, QWidget *parent=0);
  ~PViewer();
  void showTracings(QString txt);
  void showOverlay(QString txt);
  virtual void keyPressEvent(QKeyEvent *) override;
private:
  QVector<QLabel *> panels;
  class QGridLayout *grid;
  class Projection *proj;
  class Voxmap *vm;
  class IDmap *im;
};

#endif
