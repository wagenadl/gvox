// SubstackDialog.h

#ifndef SUBSTACKDIALOG_H

#define SUBSTACKDIALOG_H

#include <QDialog>

#include "Viewer.h"
#include "Transform3.h"

class SubstackDialog: public QDialog {
public:
  SubstackDialog(Viewer *viewer, QWidget *parent=0);
  ~SubstackDialog();
  void activate(Transform3 base);
private:
  class SDData *d;
};

#endif
