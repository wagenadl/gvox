// Projection.cpp

#include "Projection.h"
#include "Voxmap.h"
#include "IDmap.h"
#include "DistinctColors.h"
#include <thread>

Projection::Projection(Voxmap const *voxmap, IDmap const *idmap):
  voxmap(voxmap), idmap(idmap) {
}

QImage Projection::flatids(int ax, int dir, int xdir, int ydir) {
  if (!idmap)
    return QImage();
  
  /* X,Y are in output image; x,y,z in input stack. */
  static DistinctColors dc;
  int NX, NY, NZ; // output coordinates
  int xstride, ystride, zstride;
  switch (ax) {
  case 0: // project along x-axis; X=z, Y=y.
    NX = idmap->depth();
    xstride = idmap->width()*idmap->height();
    NY = idmap->height();
    ystride = idmap->width();
    NZ = idmap->width();
    zstride = 1;
    break;
  case 1: // project along y-axis; X=x, Y=z.
    NX = idmap->width();
    xstride = 1;
    NY = idmap->depth();
    ystride = idmap->width()*idmap->height();
    NZ = idmap->height();
    zstride = idmap->width();
    break;
  case 2: // project along z-axis; X=x, Y=y.
    NX = idmap->width();
    xstride = 1;
    NY = idmap->height();
    ystride = idmap->width();
    NZ = idmap->depth();
    zstride = idmap->width() * idmap->height();
    break;
  default:
    return QImage();
  }

  uint16_t const *src = idmap->bits();
  if (dir<0) {
    src += (NZ-1)*zstride;
    zstride = -zstride;
  }
  if (xdir<0) {
    src += (NX-1)*xstride;
    xstride = -xstride;
  }
  if (ydir<0) {
    src += (NY-1)*ystride;
    ystride = -ystride;
  }

  QImage img(NX, NY, QImage::Format_RGB32);

  for (int Y=0; Y<NY; Y++) {
    uint32_t *dst = (uint32_t*)img.scanLine(Y);
    uint16_t const *xsrc = src;
    for (int X=0; X<NX; X++) {
      uint32_t col = 0;
      uint16_t const *zsrc = xsrc;
      for (int Z=0; Z<NZ; Z++) {
	if (*zsrc)
	  col = dc.color(*zsrc);
	zsrc += zstride;
      }
      *dst++ = col;
      xsrc += xstride;
    }
    src += ystride;
  }

  return img;
}

static uint8_t *projlut() {
  uint8_t *lut = new uint8_t[256]; // sorry, memory leak
  for (int x=0; x<256; x++) {
    double y = x/255.0;
    y -= .1;
    if (y<0)
      y = 0;
    y = pow(y/.75, .5);
    if (y<0)
      y = 0;
    else if (y>1)
      y = 1;
    lut[x] = uint8_t(255.99*y);
  }
  return lut;
}
  

QImage Projection::overlay(int ax, int dir, int xdir, int ydir) {
  static uint8_t *lut = projlut();
  static DistinctColors dc;

  if (!idmap || !voxmap)
    return QImage();
  
  /* X,Y are in output image; x,y,z in voxmap.
     IDmap coordinates are half of voxmap coordinates. */

  int NX, NY, NZ; // output coordinates
  int xstride, ystride, zstride;
  int xistride, yistride, zistride;
  switch (ax) {
  case 0: // project along x-axis; X=z, Y=y.
    NX = voxmap->depth() & ~1;
    xstride = voxmap->width()*voxmap->height();
    xistride = idmap->width()*idmap->height();
    NY = voxmap->height() & ~1;
    ystride = voxmap->width();
    yistride = idmap->width();
    NZ = voxmap->width() & ~1;
    zstride = 1;
    zistride = 1;
    break;
  case 1: // project along y-axis; X=x, Y=z.
    NX = voxmap->width() & ~1;
    xstride = 1;
    xistride = 1;
    NY = voxmap->depth() & ~1;
    ystride = voxmap->width()*voxmap->height();
    yistride = idmap->width()*idmap->height();
    NZ = voxmap->height() & ~1;
    zstride = voxmap->width();
    zistride = idmap->width();
    break;
  case 2: // project along z-axis; X=x, Y=y.
    NX = voxmap->width() & ~1;
    xstride = 1;
    xistride = 1;
    NY = voxmap->height() & ~1;
    ystride = voxmap->width();
    yistride = idmap->width();
    NZ = voxmap->depth() & ~1;
    zstride = voxmap->width() * voxmap->height();
    zistride = idmap->width() * idmap->height();
    break;
  default:
    return QImage();
  }

  qDebug() << NX << NY << NZ << "/" << xstride << ystride << zstride << "/" << xistride << yistride << zistride;

  uint8_t const *src = voxmap->bits();
  uint16_t const *isrc = idmap->bits();
  
  qDebug() << isrc << idmap->width() << idmap->height() << idmap->depth();
  if (dir<0) {
    src += (NZ-1)*zstride;
    zstride = -zstride;
    isrc += (NZ/2-1)*zistride;
    zistride = -zistride;
  }
  qDebug() << isrc;
  if (xdir<0) {
    src += (NX-1)*xstride;
    xstride = -xstride;
    isrc += (NX/2-1)*xistride;
    xistride = -xistride;
  }
  if (ydir<0) {
    src += (NY-1)*ystride;
    ystride = -ystride;
    isrc += (NY/2-1)*yistride;
    yistride = -yistride;
  }
    

  QImage img(NX, NY, QImage::Format_RGB32);

  auto foo = [&](int Y0, int Y1) {
    qDebug() << "start" << Y0 << Y1;
    uint8_t const *ysrc = src + Y0*ystride;
    uint16_t const *yisrc = isrc + Y0/2*yistride;
    for (int Y=Y0; Y<Y1; Y++) {
      uint32_t *dst = (uint32_t*)img.scanLine(Y);
      uint8_t const *xsrc = ysrc;
      uint16_t const *xisrc = yisrc;
      for (int X=0; X<NX; X++) {
	float r=0, g=0, b=0;
	// float a=1;
	uint8_t const *zsrc = xsrc;
	uint16_t const *zisrc = xisrc;
	for (int Z=0; Z<NZ; Z++) {
	  float gry = lut[*zsrc] / 255.0;
	  float rh = gry;
	  float gh = gry;
	  float bh = gry;
	  float ah = gry*.030;
	  if (*zisrc) {
	    // recolor
	    uint32_t rgb = dc.color(*zisrc);
	    ah = 1;
	    rh *= ah*((rgb>>16)&255) / 255.;
	    gh *= ah*((rgb>>8)&255) / 255.;
	    bh *= ah*((rgb)&255) / 255.;
	  } else {
	    // dull?
	    rh *= ah;
	    gh *= ah;
	    bh *= ah;
	  }
	  // alpha blend
	  r = rh + r * (1-ah);
	  g = gh + g * (1-ah);
	  b = bh + b * (1-ah);
	  
	  zsrc += zstride;
	  if (Z&1)
	    zisrc += zistride;
	}
	r *= 1.5;
	g *= 1.5;
	b *= 1.5;
	if (r>1)
	  r=1;
	if (g>1)
	  g=1;
	if (b>1)
	  b=1;
	uint8_t ri(r*255.99);
	uint8_t gi(g*255.99);
	uint8_t bi(b*255.99);
	*dst++ = 0xff000000 + (ri<<16) + (gi<<8) + bi;
	xsrc += xstride;
	if (X&1)
	  xisrc += xistride;
      }
      ysrc += ystride;
      if (Y&1)
	yisrc += yistride;
    }
    qDebug() << "done" << Y0 << Y1;
  };

  int nthreads = 4;
  std::thread *thr[nthreads];
  for (int i=0; i<nthreads; i++) {
    int y0 = NY*i/nthreads/2;
    int y1 = NY*(i+1)/nthreads/2;
    thr[i] = new std::thread{foo, y0*2, y1*2};
  }
  for (int i=0; i<nthreads; i++) {
    thr[i]->join();
  }
  for (int i=0; i<nthreads; i++) {
    delete thr[i];
  }

  return img;
}

QImage Projection::projection(int ax, int dir, int xdir, int ydir) {
  static uint8_t *lut = projlut();
  static DistinctColors dc;

  if (!voxmap)
    return QImage();
  
  /* X,Y are in output image; x,y,z in voxmap.
   */

  int NX, NY, NZ; // output coordinates
  int xstride, ystride, zstride;
  switch (ax) {
  case 0: // project along x-axis; X=z, Y=y.
    NX = voxmap->depth() & ~1;
    xstride = voxmap->width()*voxmap->height();
    NY = voxmap->height() & ~1;
    ystride = voxmap->width();
    NZ = voxmap->width() & ~1;
    zstride = 1;
    break;
  case 1: // project along y-axis; X=x, Y=z.
    NX = voxmap->width() & ~1;
    xstride = 1;
    NY = voxmap->depth() & ~1;
    ystride = voxmap->width()*voxmap->height();
    NZ = voxmap->height() & ~1;
    zstride = voxmap->width();
    break;
  case 2: // project along z-axis; X=x, Y=y.
    NX = voxmap->width() & ~1;
    xstride = 1;
    NY = voxmap->height() & ~1;
    ystride = voxmap->width();
    NZ = voxmap->depth() & ~1;
    zstride = voxmap->width() * voxmap->height();
    break;
  default:
    return QImage();
  }

  uint8_t const *src = voxmap->bits();

  if (dir<0) {
    src += (NZ-1)*zstride;
    zstride = -zstride;
  }
  if (xdir<0) {
    src += (NX-1)*xstride;
    xstride = -xstride;
  }
  if (ydir<0) {
    src += (NY-1)*ystride;
    ystride = -ystride;
  }
    

  QImage img(NX, NY, QImage::Format_RGB32);

  auto foo = [&](int Y0, int Y1) {
    qDebug() << "start" << Y0 << Y1;
    uint8_t const *ysrc = src + Y0*ystride;
    for (int Y=Y0; Y<Y1; Y++) {
      uint32_t *dst = (uint32_t*)img.scanLine(Y);
      uint8_t const *xsrc = ysrc;
      for (int X=0; X<NX; X++) {
	float r=0, g=0, b=0;
	// float a=1;
	uint8_t const *zsrc = xsrc;
	for (int Z=0; Z<NZ; Z++) {
	  float gry = lut[*zsrc] / 255.0;
	  float rh = gry;
	  float gh = gry;
	  float bh = gry;
	  float ah = gry*.030;
          // dull?
          rh *= ah;
          gh *= ah;
          bh *= ah;

	  // alpha blend
	  r = rh + r * (1-ah);
	  g = gh + g * (1-ah);
	  b = bh + b * (1-ah);
	  
	  zsrc += zstride;
	}
	r *= 1.5;
	g *= 1.5;
	b *= 1.5;
	if (r>1)
	  r=1;
	if (g>1)
	  g=1;
	if (b>1)
	  b=1;
	uint8_t ri(r*255.99);
	uint8_t gi(g*255.99);
	uint8_t bi(b*255.99);
	*dst++ = 0xff000000 + (ri<<16) + (gi<<8) + bi;
	xsrc += xstride;
      }
      ysrc += ystride;
    }
  };

  int nthreads = 4;
  std::thread *thr[nthreads];
  for (int i=0; i<nthreads; i++) {
    int y0 = NY*i/nthreads/2;
    int y1 = NY*(i+1)/nthreads/2;
    thr[i] = new std::thread{foo, y0*2, y1*2};
  }
  for (int i=0; i<nthreads; i++) {
    thr[i]->join();
  }
  for (int i=0; i<nthreads; i++) {
    delete thr[i];
  }

  return img;
}
