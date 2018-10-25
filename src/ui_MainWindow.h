/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Viewer.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QAction *actionDraw_new_object;
    QAction *actionDelete_object;
    QAction *actionSelect_object;
    QAction *actionAbout;
    QAction *actionDocumentation;
    QAction *actionVentral;
    QAction *actionDorsal;
    QAction *actionLeft;
    QAction *actionRight;
    QAction *actionAnterior;
    QAction *actionPosterior;
    QAction *actionORight;
    QAction *actionOVentral;
    QAction *actionOAnterior;
    QAction *actionOLeft;
    QAction *actionODorsal;
    QAction *actionOPosterior;
    QAction *actionEORight;
    QAction *actionEOVentral;
    QAction *actionEOAnterior;
    QAction *actionEOLeft;
    QAction *actionEODorsal;
    QAction *actionEOPosterior;
    QAction *actionExport;
    QAction *actionCopy;
    QAction *actionPRight;
    QAction *actionPVentral;
    QAction *actionPAnterior;
    QAction *actionPLeft;
    QAction *actionPDorsal;
    QAction *actionPPosterior;
    QAction *actionImport;
    Viewer *viewer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QMenu *menuProjection;
    QMenu *menuOverlay;
    QMenu *menuEOverlay;
    QMenu *menuProjection_2;
    QMenu *menuEdit;
    QStatusBar *statusbar;
    QDockWidget *modes;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *select;
    QRadioButton *add;
    QRadioButton *erase;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *sall;
    QRadioButton *snamed;
    QRadioButton *sanon;
    QRadioButton *snone;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QSlider *whiteSlider;
    QSlider *gammaSlider;
    QSlider *blackSlider;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *blackValue;
    QLabel *whiteValue;
    QLabel *gammaValue;
    QSpacerItem *verticalSpacer;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *pnew;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QLineEdit *pfind;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *id;
    QToolButton *pdel;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *name;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1237, 1274);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionDraw_new_object = new QAction(MainWindow);
        actionDraw_new_object->setObjectName(QStringLiteral("actionDraw_new_object"));
        actionDelete_object = new QAction(MainWindow);
        actionDelete_object->setObjectName(QStringLiteral("actionDelete_object"));
        actionSelect_object = new QAction(MainWindow);
        actionSelect_object->setObjectName(QStringLiteral("actionSelect_object"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionDocumentation = new QAction(MainWindow);
        actionDocumentation->setObjectName(QStringLiteral("actionDocumentation"));
        actionVentral = new QAction(MainWindow);
        actionVentral->setObjectName(QStringLiteral("actionVentral"));
        actionDorsal = new QAction(MainWindow);
        actionDorsal->setObjectName(QStringLiteral("actionDorsal"));
        actionLeft = new QAction(MainWindow);
        actionLeft->setObjectName(QStringLiteral("actionLeft"));
        actionRight = new QAction(MainWindow);
        actionRight->setObjectName(QStringLiteral("actionRight"));
        actionAnterior = new QAction(MainWindow);
        actionAnterior->setObjectName(QStringLiteral("actionAnterior"));
        actionPosterior = new QAction(MainWindow);
        actionPosterior->setObjectName(QStringLiteral("actionPosterior"));
        actionORight = new QAction(MainWindow);
        actionORight->setObjectName(QStringLiteral("actionORight"));
        actionOVentral = new QAction(MainWindow);
        actionOVentral->setObjectName(QStringLiteral("actionOVentral"));
        actionOAnterior = new QAction(MainWindow);
        actionOAnterior->setObjectName(QStringLiteral("actionOAnterior"));
        actionOLeft = new QAction(MainWindow);
        actionOLeft->setObjectName(QStringLiteral("actionOLeft"));
        actionODorsal = new QAction(MainWindow);
        actionODorsal->setObjectName(QStringLiteral("actionODorsal"));
        actionOPosterior = new QAction(MainWindow);
        actionOPosterior->setObjectName(QStringLiteral("actionOPosterior"));
        actionEORight = new QAction(MainWindow);
        actionEORight->setObjectName(QStringLiteral("actionEORight"));
        actionEOVentral = new QAction(MainWindow);
        actionEOVentral->setObjectName(QStringLiteral("actionEOVentral"));
        actionEOAnterior = new QAction(MainWindow);
        actionEOAnterior->setObjectName(QStringLiteral("actionEOAnterior"));
        actionEOLeft = new QAction(MainWindow);
        actionEOLeft->setObjectName(QStringLiteral("actionEOLeft"));
        actionEODorsal = new QAction(MainWindow);
        actionEODorsal->setObjectName(QStringLiteral("actionEODorsal"));
        actionEOPosterior = new QAction(MainWindow);
        actionEOPosterior->setObjectName(QStringLiteral("actionEOPosterior"));
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        actionPRight = new QAction(MainWindow);
        actionPRight->setObjectName(QStringLiteral("actionPRight"));
        actionPVentral = new QAction(MainWindow);
        actionPVentral->setObjectName(QStringLiteral("actionPVentral"));
        actionPAnterior = new QAction(MainWindow);
        actionPAnterior->setObjectName(QStringLiteral("actionPAnterior"));
        actionPLeft = new QAction(MainWindow);
        actionPLeft->setObjectName(QStringLiteral("actionPLeft"));
        actionPDorsal = new QAction(MainWindow);
        actionPDorsal->setObjectName(QStringLiteral("actionPDorsal"));
        actionPPosterior = new QAction(MainWindow);
        actionPPosterior->setObjectName(QStringLiteral("actionPPosterior"));
        actionImport = new QAction(MainWindow);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        viewer = new Viewer(MainWindow);
        viewer->setObjectName(QStringLiteral("viewer"));
        MainWindow->setCentralWidget(viewer);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1237, 39));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuProjection = new QMenu(menuView);
        menuProjection->setObjectName(QStringLiteral("menuProjection"));
        menuOverlay = new QMenu(menuView);
        menuOverlay->setObjectName(QStringLiteral("menuOverlay"));
        menuEOverlay = new QMenu(menuView);
        menuEOverlay->setObjectName(QStringLiteral("menuEOverlay"));
        menuProjection_2 = new QMenu(menuView);
        menuProjection_2->setObjectName(QStringLiteral("menuProjection_2"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        modes = new QDockWidget(MainWindow);
        modes->setObjectName(QStringLiteral("modes"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, 6, 0, 6);
        select = new QRadioButton(groupBox);
        select->setObjectName(QStringLiteral("select"));
        select->setChecked(true);

        verticalLayout_2->addWidget(select);

        add = new QRadioButton(groupBox);
        add->setObjectName(QStringLiteral("add"));

        verticalLayout_2->addWidget(add);

        erase = new QRadioButton(groupBox);
        erase->setObjectName(QStringLiteral("erase"));

        verticalLayout_2->addWidget(erase);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(dockWidgetContents);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(6, 6, 0, 6);
        sall = new QRadioButton(groupBox_2);
        sall->setObjectName(QStringLiteral("sall"));
        sall->setChecked(true);

        verticalLayout_3->addWidget(sall);

        snamed = new QRadioButton(groupBox_2);
        snamed->setObjectName(QStringLiteral("snamed"));

        verticalLayout_3->addWidget(snamed);

        sanon = new QRadioButton(groupBox_2);
        sanon->setObjectName(QStringLiteral("sanon"));

        verticalLayout_3->addWidget(sanon);

        snone = new QRadioButton(groupBox_2);
        snone->setObjectName(QStringLiteral("snone"));

        verticalLayout_3->addWidget(snone);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(dockWidgetContents);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        whiteSlider = new QSlider(groupBox_3);
        whiteSlider->setObjectName(QStringLiteral("whiteSlider"));
        whiteSlider->setMaximum(255);
        whiteSlider->setValue(255);
        whiteSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(whiteSlider, 1, 1, 1, 1);

        gammaSlider = new QSlider(groupBox_3);
        gammaSlider->setObjectName(QStringLiteral("gammaSlider"));
        gammaSlider->setMinimum(-99);
        gammaSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(gammaSlider, 3, 1, 1, 1);

        blackSlider = new QSlider(groupBox_3);
        blackSlider->setObjectName(QStringLiteral("blackSlider"));
        blackSlider->setMaximum(255);
        blackSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(blackSlider, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 3, 0, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        blackValue = new QLabel(groupBox_3);
        blackValue->setObjectName(QStringLiteral("blackValue"));
        blackValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(blackValue, 0, 2, 1, 1);

        whiteValue = new QLabel(groupBox_3);
        whiteValue->setObjectName(QStringLiteral("whiteValue"));
        whiteValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(whiteValue, 1, 2, 1, 1);

        gammaValue = new QLabel(groupBox_3);
        gammaValue->setObjectName(QStringLiteral("gammaValue"));
        gammaValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(gammaValue, 3, 2, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        widget_3 = new QWidget(dockWidgetContents);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        pnew = new QToolButton(widget_3);
        pnew->setObjectName(QStringLiteral("pnew"));

        horizontalLayout_3->addWidget(pnew);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        pfind = new QLineEdit(widget_3);
        pfind->setObjectName(QStringLiteral("pfind"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pfind->sizePolicy().hasHeightForWidth());
        pfind->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pfind);


        verticalLayout->addWidget(widget_3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        widget = new QWidget(dockWidgetContents);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        id = new QLabel(widget);
        id->setObjectName(QStringLiteral("id"));
        id->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(id);

        pdel = new QToolButton(widget);
        pdel->setObjectName(QStringLiteral("pdel"));

        horizontalLayout->addWidget(pdel);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(dockWidgetContents);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        name = new QLineEdit(widget_2);
        name->setObjectName(QStringLiteral("name"));

        horizontalLayout_2->addWidget(name);


        verticalLayout->addWidget(widget_2);

        modes->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), modes);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionImport);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExport);
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionDocumentation);
        menuView->addAction(menuProjection->menuAction());
        menuView->addAction(menuOverlay->menuAction());
        menuView->addAction(menuProjection_2->menuAction());
        menuView->addAction(menuEOverlay->menuAction());
        menuProjection->addAction(actionRight);
        menuProjection->addAction(actionVentral);
        menuProjection->addAction(actionAnterior);
        menuProjection->addAction(actionLeft);
        menuProjection->addAction(actionDorsal);
        menuProjection->addAction(actionPosterior);
        menuOverlay->addAction(actionORight);
        menuOverlay->addAction(actionOVentral);
        menuOverlay->addAction(actionOAnterior);
        menuOverlay->addAction(actionOLeft);
        menuOverlay->addAction(actionODorsal);
        menuOverlay->addAction(actionOPosterior);
        menuEOverlay->addAction(actionEORight);
        menuEOverlay->addAction(actionEOVentral);
        menuEOverlay->addAction(actionEOAnterior);
        menuEOverlay->addAction(actionEOLeft);
        menuEOverlay->addAction(actionEODorsal);
        menuEOverlay->addAction(actionEOPosterior);
        menuProjection_2->addAction(actionPRight);
        menuProjection_2->addAction(actionPVentral);
        menuProjection_2->addAction(actionPAnterior);
        menuProjection_2->addAction(actionPLeft);
        menuProjection_2->addAction(actionPDorsal);
        menuProjection_2->addAction(actionPPosterior);
        menuEdit->addAction(actionCopy);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open...", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", Q_NULLPTR));
        actionDraw_new_object->setText(QApplication::translate("MainWindow", "Draw new object", Q_NULLPTR));
        actionDelete_object->setText(QApplication::translate("MainWindow", "Delete object", Q_NULLPTR));
        actionSelect_object->setText(QApplication::translate("MainWindow", "Select object", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        actionDocumentation->setText(QApplication::translate("MainWindow", "Documentation", Q_NULLPTR));
        actionVentral->setText(QApplication::translate("MainWindow", "Ventral", Q_NULLPTR));
        actionDorsal->setText(QApplication::translate("MainWindow", "Dorsal", Q_NULLPTR));
        actionLeft->setText(QApplication::translate("MainWindow", "Left", Q_NULLPTR));
        actionRight->setText(QApplication::translate("MainWindow", "Right", Q_NULLPTR));
        actionAnterior->setText(QApplication::translate("MainWindow", "Anterior", Q_NULLPTR));
        actionPosterior->setText(QApplication::translate("MainWindow", "Posterior", Q_NULLPTR));
        actionORight->setText(QApplication::translate("MainWindow", "Right", Q_NULLPTR));
        actionOVentral->setText(QApplication::translate("MainWindow", "Ventral", Q_NULLPTR));
        actionOAnterior->setText(QApplication::translate("MainWindow", "Anterior", Q_NULLPTR));
        actionOLeft->setText(QApplication::translate("MainWindow", "Left", Q_NULLPTR));
        actionODorsal->setText(QApplication::translate("MainWindow", "Dorsal", Q_NULLPTR));
        actionOPosterior->setText(QApplication::translate("MainWindow", "Posterior", Q_NULLPTR));
        actionEORight->setText(QApplication::translate("MainWindow", "Right", Q_NULLPTR));
        actionEOVentral->setText(QApplication::translate("MainWindow", "Ventral", Q_NULLPTR));
        actionEOAnterior->setText(QApplication::translate("MainWindow", "Anterior", Q_NULLPTR));
        actionEOLeft->setText(QApplication::translate("MainWindow", "Left", Q_NULLPTR));
        actionEODorsal->setText(QApplication::translate("MainWindow", "Dorsal", Q_NULLPTR));
        actionEOPosterior->setText(QApplication::translate("MainWindow", "Posterior", Q_NULLPTR));
        actionExport->setText(QApplication::translate("MainWindow", "Export", Q_NULLPTR));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy image to clipboard", Q_NULLPTR));
        actionPRight->setText(QApplication::translate("MainWindow", "Right", Q_NULLPTR));
        actionPVentral->setText(QApplication::translate("MainWindow", "Ventral", Q_NULLPTR));
        actionPAnterior->setText(QApplication::translate("MainWindow", "Anterior", Q_NULLPTR));
        actionPLeft->setText(QApplication::translate("MainWindow", "Left", Q_NULLPTR));
        actionPDorsal->setText(QApplication::translate("MainWindow", "Dorsal", Q_NULLPTR));
        actionPPosterior->setText(QApplication::translate("MainWindow", "Posterior", Q_NULLPTR));
        actionImport->setText(QApplication::translate("MainWindow", "Import...", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("MainWindow", "View", Q_NULLPTR));
        menuProjection->setTitle(QApplication::translate("MainWindow", "Traces", Q_NULLPTR));
        menuOverlay->setTitle(QApplication::translate("MainWindow", "Overlay", Q_NULLPTR));
        menuEOverlay->setTitle(QApplication::translate("MainWindow", "Exclusive overlay", Q_NULLPTR));
        menuProjection_2->setTitle(QApplication::translate("MainWindow", "Projection", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Mouse action:", Q_NULLPTR));
        select->setText(QApplication::translate("MainWindow", "Select object", Q_NULLPTR));
        add->setText(QApplication::translate("MainWindow", "Add to object", Q_NULLPTR));
        erase->setText(QApplication::translate("MainWindow", "Erase pixels", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Show:", Q_NULLPTR));
        sall->setText(QApplication::translate("MainWindow", "All objects", Q_NULLPTR));
        snamed->setText(QApplication::translate("MainWindow", "Named objects", Q_NULLPTR));
        sanon->setText(QApplication::translate("MainWindow", "Unnamed objects", Q_NULLPTR));
        snone->setText(QApplication::translate("MainWindow", "Nothing", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Brightness and contrast:", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "White", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Gamma", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Black", Q_NULLPTR));
        blackValue->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        whiteValue->setText(QApplication::translate("MainWindow", "255", Q_NULLPTR));
        gammaValue->setText(QApplication::translate("MainWindow", "1.0", Q_NULLPTR));
        pnew->setText(QApplication::translate("MainWindow", "New", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Go to:", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Obj ID:", Q_NULLPTR));
        id->setText(QApplication::translate("MainWindow", "01", Q_NULLPTR));
        pdel->setText(QApplication::translate("MainWindow", "Delete", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Name:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
