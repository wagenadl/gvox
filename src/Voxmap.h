// Voxmap.h

#ifndef VOXMAP_H

#define VOXMAP_H

#include <stdint.h>
#include <QJsonObject>
#include "Point.h"
#include "Transform3.h"

class Voxmap {
public:
  static constexpr int MAXMVOX = 2000;
public:
  Voxmap();
  ~Voxmap();
  bool loadFromJson(QString jsonfn);
  bool importDir(QString sourcedir, QString outbase);
  bool saveJson(QString jsonfn); // does not save raw data
  void clear();
  uint8_t const *bits() const { return data; } // use with care!
  QString name(int id) const;
  void setName(int id, QString n);
  int find(QString name) const;
  double metaValue(QString jsonname) const;
  bool hasMetaValue(QString jsonname) const;
  Point3 umtopix(Point3 um) const;
  Point3 pixtoum(Point3 px) const;
  Transform3 const &pixtoumTransform() const { return px2um; }
public:
  QString basename() const;
  int width() const { return X; }
  int height() const { return Y; }
  int depth() const { return Z; }
  QString label(QString ax) const;
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
    int x0 = floor(x);
    int y0 = floor(y);
    int z0 = floor(z);
    x -= x0;
    y -= y0;
    z -= z0;
    float x1 = 1-x;
    float y1 = 1-y;
    float z1 = 1-z;
    float px000 = pixelAt(x0,y0,z0);
    float px001 = pixelAt(x0,y0,z0+1);
    float px010 = pixelAt(x0,y0+1,z0);
    float px011 = pixelAt(x0,y0+1,z0+1);
    float px100 = pixelAt(x0+1,y0,z0);
    float px101 = pixelAt(x0+1,y0,z0+1);
    float px110 = pixelAt(x0+1,y0+1,z0);
    float px111 = pixelAt(x0+1,y0+1,z0+1);
    float x0a = 128*x;
    float y0a = 128*y;
    float z0a = 128*z;
    float x1a = 128*x1;
    float y1a = 128*y1;
    float z1a = 128*z1;
    float px00 = (z1a*px000 + z0a*px001) / 128;
    float px01 = (z1a*px010 + z0a*px011) / 128;
    float px10 = (z1a*px100 + z0a*px101) / 128;
    float px11 = (z1a*px110 + z0a*px111) / 128;
    float px0 = (y1a*px00 + y0a*px01) / 128;
    float px1 = (y1a*px10 + y0a*px11) / 128;
    return (x1a*px0 + x0a*px1) / 128;
  }
  void scanLineTril(Transform3 const &t, int y, int z, int nx,
		    uint8_t *dest, uint8_t const *lut);
  void scanLineTrilDepth8(Transform3 const &t, int y, int nx, int nz,
			 uint8_t *dest, uint8_t const *lut);
  void scanLineTrilDepth(Transform3 const &t, int y, int nx, int nz,
			 uint32_t *dest, uint32_t const *lut); // RGB version, deep lut
private:
  static void traverse(QString dir, QStringList &out);
private:
  int X, Y, Z;
  int ystride, zstride;
  uint8_t *data;
  QVector<uint8_t> store;
  uint8_t nullval;
  QJsonObject meta;
  QJsonObject names;
  QString jsonFilename;
  Transform3 um2px, px2um;
};

#endif
