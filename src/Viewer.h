// Viewer.h

#ifndef VIEWER_H

#define VIEWER_H

#include "Transform3.h"
#include <QLabel>

class Viewer: public QLabel {
  Q_OBJECT;
public:
  enum Mode {
    Select,
    Draw,
    Erase
  };
  enum View {
    All,
    Named,
    Anon,
    None
  };
public:
  Viewer(QWidget *parent=0);
  ~Viewer();
  void setVoxmap(class Voxmap *voxmap);
  void setIDmap(class IDmap *voxmap, int factor);
  int id() const { return paintid; }
  Transform3 currentTransform() const { return t; }
  View currentView() const { return view; }
  Voxmap const *currentVoxmap() const { return voxmap; }
  IDmap const *currentIDmap() const { return idmap; }
  int hiDPI() const { return hidpi_; }
public slots:
  void setMode(Mode);
  void setTransform(Transform3);
  void setName(QString);
  void doExport();
  void showEOverlay(int side, QString id);
  void showOverlay(int side);
  void showProjection(int side);
  void showTraces(int side);
  void copy();
  void add();
  void del();
  void setView(View);
  void find(int id);
  bool find(QString name);
  void buildLUT(uint8_t blk=0, uint8_t wht=255, float gamma=1);
  void gotoCenter();
  void gotoXYZum(double x, double y, double z);
  void resetRotation();
  void hideMessages(bool);
signals:
  void selectionChanged(int);
  void doubleClickedAt(Point3 p, int id);
protected:
  virtual void mouseMoveEvent(QMouseEvent *) override;
  virtual void keyPressEvent(QKeyEvent *) override;
  virtual void mousePressEvent(QMouseEvent *) override;
  virtual void mouseReleaseEvent(QMouseEvent *) override;
  virtual void mouseDoubleClickEvent(QMouseEvent *) override;
  virtual void wheelEvent(QWheelEvent *) override;
  virtual void resizeEvent(QResizeEvent *) override;
  virtual void paintEvent(QPaintEvent *) override;
  void timerEvent(QTimerEvent *) override;
protected:
  void rebuild();
  void rebuildID();
  void ensurePViewer(int k);
  void gotoID(int id);
  QString axlabel(QString ax);
  void showPos(Point3 p); // data coords
  void showPos(QPoint p); // screen coords
  void drawAxis(QString ax, QColor c, Point3 v);
private:
  Transform3 t;
  int hidpi_;
  Voxmap *voxmap;
  IDmap *idmap;
  int idfactor;
  static constexpr int HALFNZ = 5;
  uint32_t lut[(HALFNZ*2+1)*256];
  QPoint dragbase;
  Transform3 t0;
  Transform3 tid;
  Qt::MouseButton dragbutton;
  Qt::KeyboardModifiers dragmods;
  QImage im0;
  uint16_t paintid;
  QMap<int, class PViewer *> pviewer;
  QString lastkey;
  QLabel *message;
  QLabel *message2;
  Mode mode;
  View view;
  bool showcross;
  bool showcompass;
  double thickmod;
};

#endif
