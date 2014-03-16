#ifndef CONTROLLERQT_H
#define CONTROLLERQT_H

#include <QtWidgets/QMainWindow>
#include "ui_controllerqt.h"

class ControllerQT : public QMainWindow
{
	Q_OBJECT

public:
	ControllerQT(QWidget *parent = 0);
	~ControllerQT();

private:
	Ui::ControllerQTClass ui;
};

#endif // CONTROLLERQT_H
