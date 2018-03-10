/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
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
    QAction *actionExport;
    QAction *actionCopy;
    Viewer *viewer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QMenu *menuProjection;
    QMenu *menuOverlay;
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
    QSpacerItem *verticalSpacer;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *pnew;
    QToolButton *pfind;
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
        MainWindow->resize(1237, 847);
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
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
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

        pfind = new QToolButton(widget_3);
        pfind->setObjectName(QStringLiteral("pfind"));

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
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExport);
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionDocumentation);
        menuView->addAction(menuProjection->menuAction());
        menuView->addAction(menuOverlay->menuAction());
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
        actionExport->setText(QApplication::translate("MainWindow", "Export", Q_NULLPTR));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy image to clipboard", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("MainWindow", "View", Q_NULLPTR));
        menuProjection->setTitle(QApplication::translate("MainWindow", "Traces", Q_NULLPTR));
        menuOverlay->setTitle(QApplication::translate("MainWindow", "Overlay", Q_NULLPTR));
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
        pnew->setText(QApplication::translate("MainWindow", "New", Q_NULLPTR));
        pfind->setText(QApplication::translate("MainWindow", "Find", Q_NULLPTR));
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
