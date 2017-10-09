// Voxmap.cpp

#include "Voxmap.h"
#include <QImage>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>

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

void Voxmap::importDir(QString source, QString outbase) {
  clear();
  
  QStringList ifns;
  traverse(source, ifns);
  qDebug() << "Input files:" << ifns;
  if (ifns.isEmpty()) {
    return;
  }

  QImage img(ifns[0]);
  X = img.width();
  Y = img.height();
  Z = ifns.size();
  if (X*Y*Z > MAXMVOX*1024*1024) {
    qDebug() << "Image too large";
    clear();
    return;
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
      return;
    }
    memcpy((void*)(data + z*zstride), (void const *)(img.bits()), X*Y);
  }

  meta["width"] = X;
  meta["height"] = Y;
  meta["depth"] = Z;
  meta["source"] = source;
  meta["outbase"] = outbase;
  meta["importdate"] = QDateTime::currentDateTime().toString();

  QJsonObject top;

  qDebug() << "Saving...";

  QFile jsonfile(outbase + ".json");
  if (jsonfile.open(QFile::WriteOnly)) {
    top["voxmap"] = meta;
    QJsonDocument json(top);
    jsonfile.write(json.toJson());
    jsonfile.close();
  } else {    
    qDebug() << "Could not write json";
  } 


  QFile datafile(outbase + ".data");
  if (datafile.open(QFile::WriteOnly)) {
    for (int z=0; z<Z; z++) {
      // qDebug() << "Writing " << z << "/" << Z;
      datafile.write((char const *)(data + z*zstride), zstride);
    }
  } else {
    qDebug() << "Could not write data";
  }
}

void Voxmap::loadFromJson(QString jsonfn) {
  clear();
  QFile jsonfh(jsonfn);
  if (jsonfh.open(QFile::ReadOnly)) {
    QJsonDocument json = QJsonDocument::fromJson(jsonfh.read(1000*1000));
    if (!json.isObject()) {
      qDebug() << "Could not read valid json from" << jsonfn;
      return;
    }
    meta = json.object()["voxmap"].toObject();
    if (meta.isEmpty()) {
      qDebug() << "Could not get voxmap info from" << jsonfn;
      return;
    }
    jsonfh.close();
  } else {
    qDebug() << "Could not read json file" << jsonfn;
    return;
  }

  QFile datafh(meta["outbase"].toString() + ".data");
  if (datafh.open(QFile::ReadOnly)) {
    X = meta["width"].toInt();
    Y = meta["height"].toInt();
    Z = meta["depth"].toInt();

    if (X*Y*Z > MAXMVOX*1024*1024) {
      qDebug() << "Image too large";
      clear();
      return;
    }
    
    ystride = X;
    zstride = X*Y;
    data = new uint8_t[X*Y*Z];
    for (int z=0; z<Z; z++) {
      //      qDebug() << "Reading " << z << "/" << Z;
      if (datafh.read((char *)(data + z*zstride), zstride) < zstride) {
	qDebug() << "Unexpected eof";
	clear();
	return;
      }
    }
    datafh.close();
  } else {
    qDebug() << "Could not read data";
    clear();
  }
}

void Voxmap::setNullValue(uint8_t v) {
  nullval = v;
}
