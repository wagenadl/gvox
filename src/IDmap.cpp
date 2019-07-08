// IDmap.cpp

#include "IDmap.h"
#include <QByteArray>
#include <string.h>
#include <QFile>

IDmap::~IDmap() {
  timerEvent(0);
  delete [] data;
}

IDmap::IDmap(int X, int Y, int Z): X(X), Y(Y), Z(Z) {
  shouldsave = false;
  startTimer(10000);
  qDebug() << "IDmap" << X << Y << Z;
  data = 0;
  valid = false;
  if (X*Y*Z > MAXMVOX*1024*1024) {
    qDebug() << "Image too large";
    return;
  }
  
  data = new uint16_t[X*Y*Z];
  // memset((void*)(data), 0, X*Y*Z*sizeof(uint16_t));
  ystride = X;
  zstride = X*Y;
}

void IDmap::clear() {
  valid = false;
}

void IDmap::load(QString ifn) {
  // Loads RLE
  constexpr int bufsize = 65536;
  QByteArray buffer;
  buffer.resize(bufsize);
  uint16_t *optr = data;
  int N = X*Y*Z;

  QFile fh(ifn);
  if (fh.open(QFile::ReadOnly)) {
    while (true) {
      int n = fh.read(buffer.data(), bufsize);
      uint16_t const *iptr = (uint16_t const *)(buffer.data());
      if (n<0) {
	qDebug() << "Read error from " << ifn;
	clear();
	return;
      } else if (n==0) {
	break;
      } else {
	n = n/4;
	while (n>0) {
	  uint16_t val = *iptr++;
	  uint16_t cnt = *iptr++;
	  n--;
	  if (cnt<=N) {
	    for (int k=0; k<cnt; k++)
	      *optr++ = val;
	    N -= cnt;
	  } else {
	    qDebug() << "Buffer overflow from " << ifn;
	    clear();
	    return;
	  }
	}
      }
    }
    if (N>0) {
      qDebug() << "Buffer underflow from " << ifn;
      clear();
      return;
    }
  } else {
    qDebug() << "Failure to open " << ifn;
    clear();
  }
}

void IDmap::save(QString ofn) const {
  // Saves RLE
  constexpr int bufsize = 65536;
  QByteArray buffer;
  buffer.resize(bufsize);
  uint16_t const *iptr = data;
  int N = X*Y*Z;

  QFile fh(ofn);
  if (fh.open(QFile::WriteOnly)) {
    while (N>0) {
      uint16_t *optr = (uint16_t*)(buffer.data());
      int n = 0;
      while (n<bufsize && N>0) {
	uint16_t val = *iptr++;
	uint16_t cnt = 1;
	N--;
	while (cnt<65530 && N>0 && *iptr == val) {
	  cnt++;
	  N--;
	  iptr++;
	}
	*optr++ = val;
	*optr++ = cnt;
	n+=4;
      }
      if (n>0)
	fh.write(buffer.data(), n);
    }
  } else {
    qDebug() << "Failed to open " << ofn << " for writing";
  }
}

void IDmap::thickScanLine(Transform3 const &t, int y, int nx, int thick,
			  uint16_t *dest, uint16_t *buf) {
  scanLine(t, y, nx, dest, 0);
  //qDebug() << "scanLineDepth";
  //qDebug() << "  " << t.apply(Point3(y, nx/2, -2));
  //qDebug() << "  " << t.apply(Point3(y, nx/2, 0));
  //qDebug() << "  " << t.apply(Point3(y, nx/2, 2));
  for (int dz=-thick; dz<=thick; dz++) {
    if (dz==0)
      continue;
    scanLine(t, y, nx, buf, dz);
    for (int x=0; x<nx; x++)
      if (buf[x]>0 && dest[x]==0)
	dest[x] = buf[x];
  }
}

void IDmap::scanLine(Transform3 const &t, int y, int nx,
		     uint16_t *dest, int dz1) {
  Point3 p0 = t.apply(Point3(0, y, dz1));
  Point3 p1 = t.apply(Point3(nx-1, y, dz1));
  float x0 = p0.x + .5;
  float y0 = p0.y + .5;
  float z0 = p0.z + .5;
  float x1 = p1.x + .5;
  float y1 = p1.y + .5;
  float z1 = p1.z + .5;
  float dx = t.m[0][0];
  float dy = t.m[1][0];
  float dz = t.m[2][0];
  if (x0>=0 && y0>=0 && z0>=0 && x0<X && y0<Y && z0<Z
      && x1>=0 && y1>=0 && z1>=0 && x1<X && y1<Y && z1<Z) {
    for (int ix=0; ix<nx; ix++) {
      *dest++ = data[int(x0)+int(y0)*ystride+int(z0)*zstride];
      x0 += dx;
      y0 += dy;
      z0 += dz;
    }
  } else {
    for (int ix=0; ix<nx; ix++) {
      *dest++ = get(x0, y0, z0);
      x0 += dx;
      y0 += dy;
      z0 += dz;
    }
  }
}

uint16_t IDmap::max() const {
  uint16_t v = 0;
  for (int x=0; x<X*Y*Z; x++) 
    if (data[x] > v)
      v = data[x];
  return v;
}

void IDmap::timerEvent(QTimerEvent *) {
  if (!shouldsave)
    return;
  shouldsave = false;
  if (savefn.isEmpty())
    return;
  qDebug() << "saving" << savefn;
  save(savefn);
}

bool IDmap::textExport(QString ofn, int f) const {
  QFile fh(ofn);
  if (fh.open(QFile::WriteOnly)) {
    QTextStream ts(&fh);
    for (int z=0; z<Z; z++) 
      for (int y=0; y<Y; y++) 
	for (int x=0; x<X; x++) 
	  if (get(x,y,z))
	    ts << x*f << " " << y*f << " " << z*f << " " << get(x,y,z) << "\n";
    fh.close();
    qDebug() << "Exported to " << ofn;
    return true;
  } else {
    qDebug() << "Failed to export to " << ofn;
    return false;
  }
}

void IDmap::drop(uint16_t id) {
  for (int x=0; x<X*Y*Z; x++)
    if (data[x]==id)
      data[x] = 0;
  saveSoon();
}

QVector<iPoint3> IDmap::extract(uint16_t id) const {
  QVector<iPoint3> pp;
  for (int z=0; z<Z; z++)
    for (int y=0; y<Y; y++)
      for (int x=0; x<X; x++)
	if (get(x,y,z)==id)
	  pp.append(iPoint3(x,y,z));
  return pp;
}


QMap<uint16_t, Point3> IDmap::centersOfErodedMass() const {
  // First, we create a copy of the data, then we do some erosion steps.
  // Then, we find COM.
  QVector<uint16_t> dat1(X*Y*Z);
  QVector<uint16_t> dat2(X*Y*Z);
  auto erode = [=](uint16_t *dst, uint16_t const *src) {
    // clear z=0 plane
    for (int x=0; x<X; x++)
      for (int y=0; y<Y; y++)
        dst[x+y*ystride] = 0;
    // clear z=Z-1 plane
    for (int x=0; x<X; x++)
      for (int y=0; y<Y; y++)
        dst[x+y*ystride+(Z-1)*zstride] = 0;
    // clear y=0 plane
    for (int x=0; x<X; x++)
      for (int z=0; z<Z; z++)
        dst[x+z*zstride] = 0;
    // clear y=Y-1 plane
    for (int x=0; x<X; x++)
      for (int z=0; z<Z; z++)
        dst[x+z*zstride+(Y-1)*ystride] = 0;
    // clear y=0 plane
    for (int y=0; y<Y; y++)
      for (int z=0; z<Z; z++)
        dst[y*ystride+z*zstride] = 0;
    // clear y=Y-1 plane
    for (int y=0; y<Y; y++)
      for (int z=0; z<Z; z++)
        dst[y+z*zstride+(X-1)] = 0;
    // erode bulk
    for (int z=1; z<Z-1; z++) {
      for (int y=1; y<Y-1; y++) {
        for (int x=1; x<X-1; x++) {
          uint16_t v = src[x+y*ystride+z*zstride];
          uint16_t vA00 = src[x+1+y*ystride+z*zstride];
          uint16_t vB00 = src[x-1+y*ystride+z*zstride];
          uint16_t v0A0 = src[x+(y+1)*ystride+z*zstride];
          uint16_t v0B0 = src[x+(y-1)*ystride+z*zstride];
          uint16_t v00A = src[x+y*ystride+(z+1)*zstride];
          uint16_t v00B = src[x+y*ystride+(z-1)*zstride];
          if (vA00==v && vB00==v && v0A0==v && v0B0==v && v00A==v && v00B==v) {
            // copy
          } else {
            v = 0;
          }
          dst[x+y*ystride+z*zstride] = v;
        }
      }
    }
  };

  qDebug() << "start erode";
  erode(dat1.data(), data);
  qDebug() << "second erode";
  erode(dat2.data(), dat1.data());
  qDebug() << "done";
  // do it a third time? probably not
  uint16_t const *dat = dat2.data();
  
  QMap<uint16_t, Point3> sum;
  QMap<uint16_t, int> cnt;

  qDebug() << "counting";
  for (int z=1; z<Z-1; z++) {
    for (int y=1; y<Y-1; y++) {
      for (int x=1; x<X-1; x++) {
        uint16_t v = dat[x + y*ystride + z*zstride];
        if (v) {
          cnt[v]++;
          sum[v] += Point3(x, y, z);
        }
      }
    }
  }
  qDebug() << "got" << sum.size() << "objects";
  for (auto it=sum.begin(); it!=sum.end(); ++it) 
    sum[it.key()] /= cnt[it.key()];
  return sum;
}
    
