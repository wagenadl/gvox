# QMake project file for gvox                  -*- mode: shell-script; -*-


TEMPLATE = app
TARGET = gvox
CONFIG += debug_and_release
QT += widgets
CONFIG += c++11

HEADERS += Voxmap.h Viewer.h Transform3.h IDmap.h DistinctColors.h
HEADERS += PViewer.h Projection.h
SOURCES += Voxmap.cpp Viewer.cpp Transform3.cpp IDmap.cpp DistinctColors.cpp
SOURCES += PViewer.cpp Projection.cpp main.cpp
HEADERS += MainWindow.h
SOURCES += MainWindow.cpp
RESOURCES += docs.qrc
FORMS += MainWindow.ui
