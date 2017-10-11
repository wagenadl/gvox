// Projection.cpp

#include "Projection.h"
#include "Voxmap.h"
#include "IDmap.h"
#include "DistinctColors.h"

Projection::Projection(Voxmap const *voxmap, IDmap const *idmap):
  voxmap(voxmap), idmap(idmap) {
}

QImage Projection::flatids(int ax, int dir) {
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

QImage Projection::overlay(int ax, int dir) {
  if (!idmap || !voxmap)
    return QImage();
  
  /* X,Y are in output image; x,y,z in voxmap.
     IDmap coordinates are half of voxmap coordinates. */
  static DistinctColors dc;
  int NX, NY, NZ; // output coordinates
  int xstride, ystride, zstride;
  int xistride, yistride, zistride;
  switch (ax) {
  case 0: // project along x-axis; X=z, Y=y.
    NX = voxmap->depth();
    xstride = voxmap->width()*voxmap->height();
    xistride = idmap->width()*idmap->height();
    NY = voxmap->height();
    ystride = voxmap->width();
    yistride = idmap->width();
    NZ = voxmap->width();
    zstride = 1;
    zistride = 1;
    break;
  case 1: // project along y-axis; X=x, Y=z.
    NX = voxmap->width();
    xstride = 1;
    xistride = 1;
    NY = voxmap->depth();
    ystride = voxmap->width()*voxmap->height();
    yistride = idmap->width()*idmap->height();
    NZ = voxmap->height();
    zstride = voxmap->width();
    zistride = idmap->width();
    break;
  case 2: // project along z-axis; X=x, Y=y.
    NX = voxmap->width();
    xstride = 1;
    xistride = 1;
    NY = voxmap->height();
    ystride = voxmap->width();
    yistride = idmap->width();
    NZ = voxmap->depth();
    zstride = voxmap->width() * voxmap->height();
    zistride = idmap->width() * idmap->height();
    break;
  default:
    return QImage();
  }

  uint8_t const *src = voxmap->bits();
  if (dir<0) {
    src += (NZ-1)*zstride;
    zstride = -zstride;
  }

  uint16_t const *isrc = idmap->bits();
  if (dir<0) {
    isrc += (NZ/2-1)*zistride;
    zistride = -zistride;
  }

  QImage img(NX, NY, QImage::Format_RGB32);

  for (int Y=0; Y<NY; Y++) {
    uint32_t *dst = (uint32_t*)img.scanLine(Y);
    uint8_t const *xsrc = src;
    uint8_t const *xisrc = isrc;
    for (int X=0; X<NX; X++) {
      float r=0, g=0, b=0, a=1;
      uint8_t const *zsrc = xsrc;
      uint16_t const *zisrc = xisrc;
      for (int Z=0; Z<NZ; Z++) {
	float gry = *zsrc / 255.0;
	float rh = gry;
	float gh = gry;
	float bh = gry;
	float ah = gry*gry*.25;
	if (*zisrc) {
	  uint32_t rgb = dc.color(*zisrc);
	  // combine
	} else {
	  // dull?
	}
	// alpha blend
	
	zsrc += zstride;
	if (Z&1)
	  zisrc += zistride;
      }
      *dst++ = col;
      xsrc += xstride;
      if (X&1)
	xisrc += xistride;
    }
    src += ystride;
    if (Y&1)
      isrc += yistride;
  }

  return img;
}
