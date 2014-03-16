/********************************************************************************
** Form generated from reading UI file 'controllerqt.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLLERQT_H
#define UI_CONTROLLERQT_H

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

class Ui_ControllerQTClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ControllerQTClass)
    {
        if (ControllerQTClass->objectName().isEmpty())
            ControllerQTClass->setObjectName(QStringLiteral("ControllerQTClass"));
        ControllerQTClass->resize(600, 400);
        menuBar = new QMenuBar(ControllerQTClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ControllerQTClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ControllerQTClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ControllerQTClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ControllerQTClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ControllerQTClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ControllerQTClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ControllerQTClass->setStatusBar(statusBar);

        retranslateUi(ControllerQTClass);

        QMetaObject::connectSlotsByName(ControllerQTClass);
    } // setupUi

    void retranslateUi(QMainWindow *ControllerQTClass)
    {
        ControllerQTClass->setWindowTitle(QApplication::translate("ControllerQTClass", "ControllerQT", 0));
    } // retranslateUi

};

namespace Ui {
    class ControllerQTClass: public Ui_ControllerQTClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLLERQT_H
