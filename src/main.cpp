// main.cpp

#include "MainWindow.h"
#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  MainWindow mw;
  if (argc>=2) {
    QString src = argv[1];
    if (src.endsWith("json")) 
      mw.open(src);
    else
      mw.import(src);
  }
  mw.show();
  return app.exec();
}
