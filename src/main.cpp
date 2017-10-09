// main.cpp

#include "Voxmap.h"
#include "Viewer.h"

#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  Voxmap voxmap;
  voxmap.loadFromJson("/home/wagenaar/Desktop/uCT-170428/voxmap.json");
  voxmap.setNullValue(200);
		   
  Viewer viewer;
  viewer.resize(768, 768);
  viewer.setVoxmap(&voxmap);
  viewer.show();
  return app.exec();
}
