// Voxmap.h

#ifndef VOXMAP_H

#define VOXMAP_H

#include <stdint.h>
#include <QJsonObject>
#include "Point.h"
#include "Transform3.h"

class Voxmap {
public:
  static constexpr int MAXMVOX = 1000;
public:
  Voxmap();
  ~Voxmap();
  void loadFromJson(QString jsonfn);
  void importDir(QString sourcedir, QString outbase);
  void saveJson(QString jsonfn); // does not save raw data
  void clear();
public:
  int width() const { return X; }
  int height() const { return Y; }
  int depth() const { return Z; }
  //  double xResoNM() const;
  //  double yResoNM() const;
  //  double zResoNM() const;
  uint8_t nullValue() const { return nullval; }
  void setNullValue(uint8_t);
  uint8_t pixelAt(int x, int y, int z) const {
    return (x<0 || x>=X || y<0 || y>=Y || z<0 || z>=Z)
      ? nullval : data[x + y*ystride + z*zstride];
  }
  uint8_t nearestPixel(Point3 const &p) const {
    return pixelAt(int(p.x+.5), int(p.y+.5), int(p.z+.5));
  }
  void scanLine(Transform3 const &t, int y, int z, int nx, uint8_t *dest,
                uint8_t const *lut);
  uint8_t trilinear(float x, float y, float z) const {
    int x0 = int(x);
    int y0 = int(y);
    int z0 = int(z);
    x -= x0;
    y -= y0;
    z -= z0;
    float x1 = 1-x;
    float y1 = 1-y;
    float z1 = 1-z;
    uint16_t px000 = pixelAt(x0,y0,z0);
    uint16_t px001 = pixelAt(x0,y0,z0+1);
    uint16_t px010 = pixelAt(x0,y0+1,z0);
    uint16_t px011 = pixelAt(x0,y0+1,z0+1);
    uint16_t px100 = pixelAt(x0+1,y0,z0);
    uint16_t px101 = pixelAt(x0+1,y0,z0+1);
    uint16_t px110 = pixelAt(x0+1,y0+1,z0);
    uint16_t px111 = pixelAt(x0+1,y0+1,z0+1);
    uint16_t x0a = 16384*x;
    uint16_t y0a = 16384*y;
    uint16_t z0a = 16384*z;
    uint16_t x1a = 16384*x1;
    uint16_t y1a = 16384*y1;
    uint16_t z1a = 16384*z1;
    uint16_t px00 = (z1a*px000 + z0a*px001) >> 14;
    uint16_t px01 = (z1a*px010 + z0a*px011) >> 14;
    uint16_t px10 = (z1a*px100 + z0a*px101) >> 14;
    uint16_t px11 = (z1a*px110 + z0a*px111) >> 14;
    uint16_t px0 = (y1a*px00 + y0a*px01) >> 14;
    uint16_t px1 = (y1a*px10 + y0a*px11) >> 14;
    return (x1a*px0 + x0a*px1) >> 14;
  }
  void scanLineTril(Transform3 const &t, int y, int z, int nx,
		    uint8_t *dest, uint8_t const *lut);
  void scanLineTrilDepth(Transform3 const &t, int y, int nx, int nz,
			 uint8_t *dest, uint8_t const *lut);
private:
  static void traverse(QString dir, QStringList &out);
private:
  int X, Y, Z;
  int ystride, zstride;
  uint8_t *data;
  uint8_t nullval;
  QJsonObject meta;
};

#endif
