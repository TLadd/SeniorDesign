#include "gui.h"

GUI::GUI()
{
}

GUI::~GUI()
{

}

void GUI::start(int argc, char **argv) {
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	a.exec();
}