// MainWindow.cpp

#include "MainWindow.h"

#include "Voxmap.h"
#include "IDmap.h"

#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_MainWindow.h"

constexpr int IDFACTOR = 2;

void showDocs() {
  QFile f(":/README.txt");
  if (!f.open(QFile::ReadOnly)) {
    qDebug() << "Docs!?";
    return;
  }
  QString txt = QString(f.readAll());
  QStringList bits = txt.split("\n\n");
  
  QString me = "<b>gvox</b>";
  QString vsn = "0.1";
  txt = me + " " + vsn
                     + "<p>" + "(C) 2018 Daniel A. Wagenaar\n"
    + "<p>" + me + " is a program for displaying and annotating gigavoxel volumes.\n";
  for (auto b: bits)
    txt += "<p>" + b;
  
  QMessageBox::about(0, me, txt);
}

void showAbout() {
  QString me = "<b>gvox</b>";
  QString vsn = "0.1";
  QMessageBox::about(0, "About " + me,
                     me + " " + vsn
                     + "<p>" + "(C) 2018 Daniel A. Wagenaar\n"
                     + "<p>" + me + " is a program for displaying and annotating gigavoxel volumes.\n"
                     + "<p>" + me + " is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n"
                     + "<p>" + me + " is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n"
                     + "<p>" + "You should have received a copy of the GNU General Public License along with this program. If not, see <a href=\"http://www.gnu.org/licenses/gpl-3.0.en.html\">www.gnu.org/licenses/gpl-3.0.en.html</a>.");
}

MainWindow::MainWindow() {
  voxmap = 0;
  idmap = 0;
  ui = new Ui_MainWindow();
  ui->setupUi(this);

  ui->id->setText("");
  ui->name->setEnabled(false);
  ui->pdel->setEnabled(false);
  
  connect(ui->actionOpen, &QAction::triggered,
          [this]() { openDialog(); });
  connect(ui->actionExport, &QAction::triggered,
          [this]() { ui->viewer->doExport(); });
  connect(ui->actionQuit, &QAction::triggered,
          []() { QApplication::quit(); });

  connect(ui->actionCopy, &QAction::triggered,
          [this]() { ui->viewer->copy(); });

  connect(ui->actionRight, &QAction::triggered,
          [this]() { ui->viewer->showTraces(1); });
  connect(ui->actionVentral, &QAction::triggered,
          [this]() { ui->viewer->showTraces(2); });
  connect(ui->actionAnterior, &QAction::triggered,
          [this]() { ui->viewer->showTraces(3); });
  connect(ui->actionLeft, &QAction::triggered,
          [this]() { ui->viewer->showTraces(4); });
  connect(ui->actionDorsal, &QAction::triggered,
          [this]() { ui->viewer->showTraces(5); });
  connect(ui->actionPosterior, &QAction::triggered,
          [this]() { ui->viewer->showTraces(6); });

  connect(ui->actionORight, &QAction::triggered,
          [this]() { ui->viewer->showOverlay(1); });
  connect(ui->actionOVentral, &QAction::triggered,
          [this]() { ui->viewer->showOverlay(2); });
  connect(ui->actionOAnterior, &QAction::triggered,
          [this]() { ui->viewer->showOverlay(3); });
  connect(ui->actionOLeft, &QAction::triggered,
          [this]() { ui->viewer->showOverlay(4); });
  connect(ui->actionODorsal, &QAction::triggered,
          [this]() { ui->viewer->showOverlay(5); });
  connect(ui->actionOPosterior, &QAction::triggered,
          [this]() { ui->viewer->showOverlay(6); });
  
  connect(ui->actionAbout, &QAction::triggered,
          []() { showAbout(); });
  connect(ui->actionDocumentation, &QAction::triggered,
          []() { showDocs(); });

  connect(ui->sall, &QRadioButton::toggled,
          [this](bool b) { if (b) ui->viewer->setView(Viewer::All); });
  connect(ui->snamed, &QRadioButton::toggled,
          [this](bool b) { if (b) ui->viewer->setView(Viewer::Named); });
  connect(ui->sanon, &QRadioButton::toggled,
          [this](bool b) { if (b) ui->viewer->setView(Viewer::Anon); });
  connect(ui->snone, &QRadioButton::toggled,
          [this](bool b) { if (b) ui->viewer->setView(Viewer::None); });
  
  connect(ui->select, &QRadioButton::toggled,
          [this](bool b) { if (b) ui->viewer->setMode(Viewer::Select); });
  connect(ui->add, &QRadioButton::toggled,
          [this](bool b) { if (b) ui->viewer->setMode(Viewer::Draw); });
  connect(ui->erase, &QRadioButton::toggled,
          [this](bool b) {
            if (b) {
              ui->viewer->setMode(Viewer::Erase);
              ui->pdel->setEnabled(ui->viewer->id()>0);
            } else {
              ui->pdel->setEnabled(false);
            }});
  connect(ui->viewer, &Viewer::selectionChanged,
          [this](int i) { ui->id->setText(i ? QString::number(i) : QString(""));
                          if (i==0) ui->select->setChecked(true);
                          ui->name->setEnabled(i>0); 
                          ui->pdel->setEnabled(i>0 && ui->erase->isChecked());
                          ui->name->setText(voxmap->name(i));
          });
  connect(ui->name, &QLineEdit::editingFinished,
          [this]() { ui->viewer->setName(ui->name->text()); });

  connect(ui->pnew, &QToolButton::clicked,
          [this]() { ui->viewer->add(); ui->add->setChecked(true); });
  connect(ui->pdel, &QToolButton::clicked,
          [this]() { ui->viewer->del(); ui->select->setChecked(true); });
  connect(ui->pfind, &QLineEdit::returnPressed,
          [this]() { findDialog(); });
}

MainWindow::~MainWindow() {
  // delete idmap;
  // delete voxmap;
}

void MainWindow::importDialog() {
  QString dir = QDir::home().absoluteFilePath("Desktop");
  QString fn = QFileDialog::getExistingDirectory(0,
                                                 "Open jpeg folder",
                                                 dir);
  if (!fn.isEmpty())
    import(fn);
}

void MainWindow::import(QString fn) {
  if (!voxmap) 
    voxmap = new Voxmap;

  voxmap->importDir(fn, fn);

  doLoad();
}

void MainWindow::openDialog() {
  QString dir = QDir::home().absoluteFilePath("Desktop");
  QString fn = QFileDialog::getOpenFileName(0,
					    "Open gvox file",
					    dir,
					    tr("JSON (*.json)"));
  if (!fn.isEmpty())
    open(fn);
}

void MainWindow::open(QString fn) {
  if (!voxmap) 
    voxmap = new Voxmap;

  voxmap->loadFromJson(fn);
  doLoad();
}

void MainWindow::doLoad() {
  voxmap->setNullValue(0);
  
  if (idmap)
    delete idmap;

  idmap = new IDmap(voxmap->width()/IDFACTOR, voxmap->height()/IDFACTOR,
                    voxmap->depth()/IDFACTOR);
  idmap->load(voxmap->basename() + ".id-rle");
  idmap->setAutoSaveName(voxmap->basename() + ".id-rle");

  ui->viewer->setVoxmap(voxmap);
  ui->viewer->setIDmap(idmap, IDFACTOR);
}
  
void MainWindow::findDialog() {
  QString txt = ui->pfind->text();
  qDebug() << "Find" << txt;
  bool ok;
  int id = txt.toInt(&ok);
  if (ok) {
    ui->viewer->find(id);
  } else {
    if (!ui->viewer->find(txt))
      qDebug() << "not found";
  }
}
