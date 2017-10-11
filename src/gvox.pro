# QMake project file for gvox                  -*- mode: shell-script; -*-


TEMPLATE = app
TARGET = gvox
CONFIG += release
QT += widgets
CONFIG += c++11

HEADERS += Voxmap.h Viewer.h Transform3.h IDmap.h DistinctColors.h
HEADERS += PViewer.h Projection.h
SOURCES += Voxmap.cpp Viewer.cpp Transform3.cpp IDmap.cpp DistinctColors.cpp
SOURCES += PViewer.cpp Projection.cpp main.cpp
