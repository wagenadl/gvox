// main.cpp

#include "Voxmap.h"
#include "Viewer.h"
#include "IDmap.h"

#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  Voxmap voxmap;
  if (argc>=2) {
    QString src = argv[1];
    if (src.endsWith("json")) 
      voxmap.loadFromJson(src);
    else
      voxmap.importDir(src, src);
  } else {
    voxmap.loadFromJson("/home/wagenaar/Desktop/uCT-170428/voxmap.json");
  }
  voxmap.setNullValue(200);

  constexpr int IDFACTOR = 2;
  IDmap idmap(voxmap.width()/IDFACTOR, voxmap.height()/IDFACTOR,
	      voxmap.depth()/IDFACTOR);
  idmap.load(voxmap.basename() + ".id-rle");
  idmap.setAutoSaveName(voxmap.basename() + ".id-rle");
		   
  Viewer viewer;
  viewer.resize(2*256*3, 2*256*3);
  viewer.setVoxmap(&voxmap);
  viewer.setIDmap(&idmap, IDFACTOR);
  viewer.show();
  return app.exec();
}
