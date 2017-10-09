// main.cpp

#include "Voxmap.h"
#include <QImage>
#include <QLabel>
#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  Voxmap voxmap;
  voxmap.importDir("/home/wagenaar/Desktop/uCT-170428/jpeg",
		   "/home/wagenaar/Desktop/uCT-170428/voxmap");
  QImage img(300, 300, QImage::Format_Grayscale8);
  uint8_t *bits = img.bits();
  int ystride = 300;
  for (int y=0; y<300; y++) {
    uint8_t *line = bits + y*ystride;
    for (int x=0; x<300; x++) 
      line[x] = voxmap.pixelAt(x+150, y+150, 250);
  }

  QLabel label;
  label.setPixmap(QPixmap::fromImage(img));
  label.show();
  return app.exec();
}
