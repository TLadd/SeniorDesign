/********************************************************************************
** Form generated from reading UI file 'vitalsoneproj.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VITALSONEPROJ_H
#define UI_VITALSONEPROJ_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VitalsOneProjClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VitalsOneProjClass)
    {
        if (VitalsOneProjClass->objectName().isEmpty())
            VitalsOneProjClass->setObjectName(QStringLiteral("VitalsOneProjClass"));
        VitalsOneProjClass->resize(600, 400);
        menuBar = new QMenuBar(VitalsOneProjClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        VitalsOneProjClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VitalsOneProjClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VitalsOneProjClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(VitalsOneProjClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        VitalsOneProjClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(VitalsOneProjClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VitalsOneProjClass->setStatusBar(statusBar);

        retranslateUi(VitalsOneProjClass);

        QMetaObject::connectSlotsByName(VitalsOneProjClass);
    } // setupUi

    void retranslateUi(QMainWindow *VitalsOneProjClass)
    {
        VitalsOneProjClass->setWindowTitle(QApplication::translate("VitalsOneProjClass", "VitalsOneProj", 0));
    } // retranslateUi

};

namespace Ui {
    class VitalsOneProjClass: public Ui_VitalsOneProjClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VITALSONEPROJ_H
