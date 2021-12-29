/********************************************************************************
** Form generated from reading UI file 'SimpleRender_x64.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLERENDER_X64_H
#define UI_SIMPLERENDER_X64_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "glWidget.h"

QT_BEGIN_NAMESPACE

class Ui_SimpleRender_x64Class
{
public:
    QAction *actionOpen;
    QAction *actionImportLight;
    QAction *actionImportPolLight;
    QAction *actionCreatePotLight;
    QAction *actionPCSS;
    QAction *actionPRT_ZH;
    QAction *actionFreeMode;
    QAction *actionCenterMode;
    QAction *actionSave;
    QAction *actionImport;
    QWidget *centralWidget;
    glWidget *openGLWidget;
    QListWidget *listWidget;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuLight;
    QMenu *menuMode;
    QMenu *menuCamera;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SimpleRender_x64Class)
    {
        if (SimpleRender_x64Class->objectName().isEmpty())
            SimpleRender_x64Class->setObjectName(QString::fromUtf8("SimpleRender_x64Class"));
        SimpleRender_x64Class->resize(1584, 1059);
        SimpleRender_x64Class->setFocusPolicy(Qt::ClickFocus);
        actionOpen = new QAction(SimpleRender_x64Class);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionImportLight = new QAction(SimpleRender_x64Class);
        actionImportLight->setObjectName(QString::fromUtf8("actionImportLight"));
        actionImportPolLight = new QAction(SimpleRender_x64Class);
        actionImportPolLight->setObjectName(QString::fromUtf8("actionImportPolLight"));
        actionCreatePotLight = new QAction(SimpleRender_x64Class);
        actionCreatePotLight->setObjectName(QString::fromUtf8("actionCreatePotLight"));
        actionPCSS = new QAction(SimpleRender_x64Class);
        actionPCSS->setObjectName(QString::fromUtf8("actionPCSS"));
        actionPRT_ZH = new QAction(SimpleRender_x64Class);
        actionPRT_ZH->setObjectName(QString::fromUtf8("actionPRT_ZH"));
        actionFreeMode = new QAction(SimpleRender_x64Class);
        actionFreeMode->setObjectName(QString::fromUtf8("actionFreeMode"));
        actionCenterMode = new QAction(SimpleRender_x64Class);
        actionCenterMode->setObjectName(QString::fromUtf8("actionCenterMode"));
        actionSave = new QAction(SimpleRender_x64Class);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionImport = new QAction(SimpleRender_x64Class);
        actionImport->setObjectName(QString::fromUtf8("actionImport"));
        centralWidget = new QWidget(SimpleRender_x64Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        openGLWidget = new glWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(100, 20, 1440, 960));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 20, 81, 961));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(110, 30, 81, 21));
        SimpleRender_x64Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SimpleRender_x64Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1584, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuLight = new QMenu(menuBar);
        menuLight->setObjectName(QString::fromUtf8("menuLight"));
        menuMode = new QMenu(menuBar);
        menuMode->setObjectName(QString::fromUtf8("menuMode"));
        menuCamera = new QMenu(menuBar);
        menuCamera->setObjectName(QString::fromUtf8("menuCamera"));
        SimpleRender_x64Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SimpleRender_x64Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        SimpleRender_x64Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SimpleRender_x64Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        SimpleRender_x64Class->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuLight->menuAction());
        menuBar->addAction(menuMode->menuAction());
        menuBar->addAction(menuCamera->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionImport);
        menuLight->addAction(actionImportPolLight);
        menuLight->addAction(actionCreatePotLight);
        menuMode->addAction(actionPCSS);
        menuMode->addAction(actionPRT_ZH);
        menuCamera->addAction(actionFreeMode);
        menuCamera->addAction(actionCenterMode);

        retranslateUi(SimpleRender_x64Class);

        QMetaObject::connectSlotsByName(SimpleRender_x64Class);
    } // setupUi

    void retranslateUi(QMainWindow *SimpleRender_x64Class)
    {
        SimpleRender_x64Class->setWindowTitle(QCoreApplication::translate("SimpleRender_x64Class", "SimpleRender_x64", nullptr));
        actionOpen->setText(QCoreApplication::translate("SimpleRender_x64Class", "Open", nullptr));
        actionImportLight->setText(QCoreApplication::translate("SimpleRender_x64Class", "ImportLight", nullptr));
        actionImportPolLight->setText(QCoreApplication::translate("SimpleRender_x64Class", "ImportPolLight", nullptr));
        actionCreatePotLight->setText(QCoreApplication::translate("SimpleRender_x64Class", "CreatePotLight", nullptr));
        actionPCSS->setText(QCoreApplication::translate("SimpleRender_x64Class", "PCSS", nullptr));
        actionPRT_ZH->setText(QCoreApplication::translate("SimpleRender_x64Class", "PRT_ZH", nullptr));
        actionFreeMode->setText(QCoreApplication::translate("SimpleRender_x64Class", "FreeMode", nullptr));
        actionCenterMode->setText(QCoreApplication::translate("SimpleRender_x64Class", "CenterMode", nullptr));
        actionSave->setText(QCoreApplication::translate("SimpleRender_x64Class", "Save", nullptr));
        actionImport->setText(QCoreApplication::translate("SimpleRender_x64Class", "Import", nullptr));
        lineEdit->setText(QCoreApplication::translate("SimpleRender_x64Class", "FPS:111", nullptr));
        menuFile->setTitle(QCoreApplication::translate("SimpleRender_x64Class", "File", nullptr));
        menuLight->setTitle(QCoreApplication::translate("SimpleRender_x64Class", "Light", nullptr));
        menuMode->setTitle(QCoreApplication::translate("SimpleRender_x64Class", "Mode", nullptr));
        menuCamera->setTitle(QCoreApplication::translate("SimpleRender_x64Class", "Camera", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimpleRender_x64Class: public Ui_SimpleRender_x64Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLERENDER_X64_H
