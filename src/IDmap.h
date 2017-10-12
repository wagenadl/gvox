// IDmap.h

#ifndef IDMAP_H

#define IDMAP_H

#include <stdint.h>
#include "Transform3.h"
#include <QObject>
#include <QVector>

class IDmap: public QObject {
  static constexpr int MAXMVOX = 1000;
public:
  IDmap(int X, int Y, int Z);
  ~IDmap();
  void set(int x, int y, int z, uint16_t id) {
    if (x<0 || x>=X || y<0 || y>=Y || z<0 || z>=Z)
      return;
    data[x + y*ystride + z*zstride] = id;
  }
  void paint(float x, float y, float z, uint16_t id) {
    set(x+.5, y+.5, z+.5, id);
    saveSoon();
  }
  void drop(uint16_t id);
  uint16_t max() const;
  uint16_t get(int x, int y, int z) const {
    return (x<0 || x>=X || y<0 || y>=Y || z<0 || z>=Z)
      ? 0 : data[x + y*ystride + z*zstride];
  }
  uint16_t getf(float x, float y, float z) const {
    return get(x+.5, y+.5, z+.5);
  }
  uint16_t const *bits() const { return data; } // use with care!
  void load(QString ifn);
  void save(QString ofn) const;
  bool textExport(QString ofn) const; // true if OK
  void setAutoSaveName(QString ofn) {
    savefn = ofn;
  }
  void saveSoon() {
    shouldsave = true;
  }
  void clear();
  void scanLine(Transform3 const &t, int y, int nx, uint16_t *dest);
  void nearestNonZero(int &x, int &y, int &z) const;
  int width() const { return X; }
  int height() const { return Y; }
  int depth() const { return Z; }
  bool isValid() const { return valid; }
  virtual void timerEvent(QTimerEvent *) override;
  QVector<iPoint3> extract(uint16_t id) const; // get all coords of ID
private:
  int X, Y, Z;
  int ystride, zstride;
  bool valid;
  uint16_t *data;
  QString savefn;
  bool shouldsave;
};

#endif
