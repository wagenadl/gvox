// main.cpp

#include "Voxmap.h"
#include <QImage>
#include <QLabel>
#include <QApplication>
#include <math.h>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  Voxmap voxmap;
  voxmap.loadFromJson("/home/wagenaar/Desktop/uCT-170428/voxmap.json");
  voxmap.setNullValue(200);
		   
  QImage img(300, 300, QImage::Format_Grayscale8);
  uint8_t lut[256];
  for (int x=0; x<256; x++) {
    double y = x/255.0;
    y = pow(y-.2, .75)/.8;
    if (y<0)
      y = 0;
    else if (y>1)
      y = 1;
    lut[x] = int(255.99*y);
  }
  
  uint8_t *bits = img.bits();
  int ystride = 300;
  for (int y=0; y<300; y++) {
    uint8_t *line = bits + y*ystride;
    for (int x=0; x<300; x++) 
      line[x] = lut[voxmap.pixelAt(x+250, y+250, 250)];
  }

  QLabel label;
  label.setPixmap(QPixmap::fromImage(img));
  label.setScaledContents(true);
  
  label.show();
  return app.exec();
}
