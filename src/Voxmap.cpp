// Voxmap.cpp

#include <QImage>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>
#include <math.h>

#include "Voxmap.h"

Voxmap::Voxmap() {
  X = Y = Z = 0;
  ystride = zstride = 0;
  data = 0;
  nullval = 0;
}

Voxmap::~Voxmap() {
  delete data;
}

void Voxmap::traverse(QString src, QStringList &out) {
  QDir dir(src);
  for (auto sub: dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot,
			       QDir::Name)) {
    traverse(dir.filePath(sub), out);
  }
  QStringList flts;
  flts << "*.jpg";
  flts << "*.jpeg";
  flts << "*.tif";
  flts << "*.tiff";
  flts << "*.png";
  for (auto fn: dir.entryList(flts, QDir::Files  | QDir::NoDotAndDotDot,
			      QDir::Name)) {
    out << dir.filePath(fn);
  }
}

void Voxmap::clear() {
  X = Y = Z = 0;
  delete [] data;
  data = 0;
  meta = QJsonObject();
}  

bool Voxmap::importDir(QString source, QString outbase) {
  clear();
  
  QStringList ifns;
  traverse(source, ifns);
  qDebug() << "Input files:" << ifns;
  if (ifns.isEmpty()) {
    return false;
  }

  QImage img(ifns[0]);
  X = img.width();
  Y = img.height();
  Z = ifns.size();
  if (X*Y*Z > MAXMVOX*1024*1024) {
    qDebug() << "Image too large";
    clear();
    return false;
  }
  
  data = new uint8_t[X*Y*Z];
  ystride = X;
  zstride = X*Y;

  for (int z=0; z<Z; z++) {
    qDebug() << "Loading " << z << "/" << Z << ": " << ifns[z];
    QImage img(ifns[z]);
    if (img.width()!=X || img.height()!=Y) {
      qDebug() << "size mismatch at " << z << ": " << ifns[z];
      clear();
      return false;
    }
    memcpy((void*)(data + z*zstride), (void const *)(img.bits()), X*Y);
  }

  meta["width"] = X;
  meta["height"] = Y;
  meta["depth"] = Z;
  meta["source"] = source;
  meta["outbase"] = outbase;
  meta["base"] = outbase;
  meta["importdate"] = QDateTime::currentDateTime().toString();
  meta["label-xpositive"] = "Right";
  meta["label-xnegative"] = "Left";
  meta["label-ypositive"] = "Down";
  meta["label-ynegative"] = "Up";
  meta["label-zpositive"] = "Back";
  meta["label-znegative"] = "Front";
  meta["label-xp"] = "R";
  meta["label-xn"] = "L";
  meta["label-yp"] = "D";
  meta["label-yn"] = "U";
  meta["label-zp"] = "B";
  meta["label-zn"] = "F";


  if (!saveJson(outbase + ".json"))
    return false;

  QFile datafile(outbase + ".data");
  if (datafile.open(QFile::WriteOnly)) {
    for (int z=0; z<Z; z++) {
      // qDebug() << "Writing " << z << "/" << Z;
      datafile.write((char const *)(data + z*zstride), zstride);
    }
  } else {
    qDebug() << "Could not write data";
  }
  return true;
}

bool Voxmap::saveJson(QString jsonfn) {
  qDebug() << "Saving...";

  QJsonObject top;
  QFile jsonfile(jsonfn);
  if (jsonfile.open(QFile::WriteOnly)) {
    top["voxmap"] = meta;
    top["names"] = names;
    QJsonDocument json(top);
    jsonfile.write(json.toJson());
    jsonfile.close();
  } else {    
    qDebug() << "Could not write json";
    return false;
  }
  return true;
}

int Voxmap::find(QString name) const {
  name = name.toLower();
  for (QString idn: names.keys()) 
    if (names[idn].toString().toLower() == name)
      return idn.toInt();
  return 0;
}

QString Voxmap::name(int id) const {
  QString idn = QString::number(id);
  if (names.contains(idn))
    return names[idn].toString();
  else
    return "";
}

void Voxmap::setName(int id, QString n) {
  if (name(id)!=n) {
    QString idn = QString::number(id);
    names[idn] = n;
    saveJson(jsonFilename);
  }
}

bool Voxmap::loadFromJson(QString jsonfn) {
  clear();
  QFile jsonfh(jsonfn);
  if (jsonfh.open(QFile::ReadOnly)) {
    QJsonDocument json = QJsonDocument::fromJson(jsonfh.read(1000*1000));
    if (!json.isObject()) {
      qDebug() << "Could not read valid json from" << jsonfn;
      return false; 
    }
    meta = json.object()["voxmap"].toObject();
    if (meta.isEmpty()) {
      qDebug() << "Could not get voxmap info from" << jsonfn;
      return false;
    }
    names = json.object()["names"].toObject();
    jsonfh.close();
    jsonFilename = jsonfn;
  } else {
    qDebug() << "Could not read json file" << jsonfn;
    return false;
  }

  QFileInfo info(jsonfn);
  QDir parent(info.dir());
  QString dirname = parent.absolutePath();
  QString base = info.baseName();
  if (parent.exists(base + ".data")) {
    meta["base"] = dirname + "/" + base;
  } else {
    meta["base"] = meta["outbase"];
  }
  
  QFile datafh(basename() + ".data");
  if (datafh.open(QFile::ReadOnly)) {
    X = meta["width"].toInt();
    Y = meta["height"].toInt();
    Z = meta["depth"].toInt();

    if (X*Y*Z > MAXMVOX*1024*1024) {
      qDebug() << "Image too large";
      clear();
      return false;
    }
    
    ystride = X;
    zstride = X*Y;
    data = new uint8_t[X*Y*Z];
    for (int z=0; z<Z; z++) {
      //      qDebug() << "Reading " << z << "/" << Z;
      if (datafh.read((char *)(data + z*zstride), zstride) < zstride) {
	qDebug() << "Unexpected eof";
	clear();
	return false;
      }
    }
    datafh.close();
  } else {
    qDebug() << "Could not read data";
    clear();
    return false;
  }
  return true;
}

void Voxmap::setNullValue(uint8_t v) {
  nullval = v;
}

void Voxmap::scanLine(Transform3 const &t, int y, int z, int nx,
                      uint8_t *dest, uint8_t const *lut) {
  Point3 p0 = t.apply(Point3(0, y, z));
  Point3 p1 = t.apply(Point3(nx-1, y, z));
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
      *dest++ = lut[data[int(x0)+int(y0)*ystride+int(z0)*zstride]];
      x0 += dx;
      y0 += dy;
      z0 += dz;
    }
  } else {
    for (int ix=0; ix<nx; ix++) {
      *dest++ = lut[pixelAt(x0, y0, z0)];
      x0 += dx;
      y0 += dy;
      z0 += dz;
    }
  }
}

void Voxmap::scanLineTril(Transform3 const &t, int y, int z, int nx,
			       uint8_t *dest, uint8_t const *lut) {
  Point3 p0 = t.apply(Point3(0, y, z));
  float x0 = p0.x;
  float y0 = p0.y;
  float z0 = p0.z;
  float dx = t.m[0][0];
  float dy = t.m[1][0];
  float dz = t.m[2][0];
  for (int ix=0; ix<nx; ix++) {
    *dest++ = lut[trilinear(x0, y0, z0)];
    x0 += dx;
    y0 += dy;
    z0 += dz;
  }
}


/* Alpha blending based on https://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending:
    outA  =  frontA  +  backA  * (1-frontA)
   outRGB = frontRGB + backRGB * (1-frontA)
 This assumes pre-multiplied alpha.
 */

void Voxmap::scanLineTrilDepth8(Transform3 const &t, int y, int nx, int nz,
			       uint8_t *dest, uint8_t const *lut) {
  Point3 p0 = t.apply(Point3(0, y, (nz-1)/2.));
  float x0 = p0.x;
  float y0 = p0.y;
  float z0 = p0.z;
  float dx = t.m[0][0];
  float dy = t.m[1][0];
  float dz = t.m[2][0];
  float dxd = -t.m[0][2];
  float dyd = -t.m[1][2];
  float dzd = -t.m[2][2];
  for (int ix=0; ix<nx; ix++) {
    float alpha = 1;
    float gray = 0;
    float x1 = x0;
    float y1 = y0;
    float z1 = z0;
    for (int iz=0; iz<nz; iz++) {
      float here = trilinear(x1, y1, z1)/255.0;
      float halpha = sqrt(here);
      gray = here*halpha + gray*(1-halpha);
      x1 += dxd;
      y1 += dyd;
      z1 += dzd;
    }
    *dest++ = lut[uint8_t(255.99*gray/alpha)];
    x0 += dx;
    y0 += dy;
    z0 += dz;
  }
}

void Voxmap::scanLineTrilDepth(Transform3 const &t, int y, int nx, int nz,
			       uint32_t *dest, uint32_t const *lut) {
  Point3 p0 = t.apply(Point3(0, y, (nz-1)/2.));
  float x0 = p0.x;
  float y0 = p0.y;
  float z0 = p0.z;
  float dx = t.m[0][0];
  float dy = t.m[1][0];
  float dz = t.m[2][0];
  float dxd = -t.m[0][2];
  float dyd = -t.m[1][2];
  float dzd = -t.m[2][2];
  for (int ix=0; ix<nx; ix++) {
    float a = 0;
    float r=0, g=0, b=0;
    float x1 = x0;
    float y1 = y0;
    float z1 = z0;
    for (int iz=0; iz<=nz/2; iz++) { // first half
      uint32_t herergb = lut[256*iz+trilinear(x1, y1, z1)]; 
     float rh = ((herergb>>16) & 255)/255.0;
      float gh = ((herergb>>8) & 255)/255.0;
      float bh = ((herergb) & 255)/255.0;
      float ah = bh*bh; // OK?
      r = rh*ah + r*(1-ah);
      g = gh*ah + g*(1-ah);
      b = bh*ah + b*(1-ah);
      a = ah + a*(1-ah);
      x1 += dxd;
      y1 += dyd;
      z1 += dzd;
    }
    for (int iz=nz/2+1; iz<nz; iz++) { // second half
      uint32_t herergb = lut[256*iz+trilinear(x1, y1, z1)];
      float rh = ((herergb>>16) & 255)/255.0;
      float gh = ((herergb>>8) & 255)/255.0;
      float bh = ((herergb) & 255)/255.0;
      float ah = bh*bh; // OK?
      r = r + rh*ah*(1-a);
      g = g + gh*ah*(1-a);
      b = b + bh*ah*(1-a);
      a = a + ah*(1-a);
      x1 += dxd;
      y1 += dyd;
      z1 += dzd;
    }
    uint8_t r1 = r*255.99;
    uint8_t g1 = g*255.99;
    uint8_t b1 = b*255.99;
    *dest++ = 0xff000000u + b1 + 256u*g1 + 65536u*r1;
    x0 += dx;
    y0 += dy;
    z0 += dz;
  }
}

QString Voxmap::basename() const {
  return meta["base"].toString();
}

QString Voxmap::label(QString ax) const {
  return meta["label-" + ax].toString();
}
