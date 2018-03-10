// MainWindow.h

#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include <QMainWindow>
#include "Voxmap.h"

class MainWindow: public QMainWindow {
public:
  MainWindow();
  virtual ~MainWindow();
public:
  void import(QString fn);
  void open(QString fn);
private:
  void openDialog();
  void importDialog();
  void doLoad();
  void findDialog();
private:
  class Ui_MainWindow *ui;
  class Voxmap *voxmap;
  class IDmap *idmap;
};

#endif
