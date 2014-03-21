
#define WIN32_LEAN_AND_MEAN
#include "GUI/mainwindow.h"
#include "Model/VitalsModel.h"
#include <QtWidgets/QApplication>
#include <boost/thread/thread.hpp>


void startModel(boost::asio::io_service *io) {
	
	VitalsModel model(*io, 20, 2, 750);
	model.start();
	io->run();
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	boost::asio::io_service io;
	thread *t = new thread(&startModel, &io);

	w.show();
	return a.exec();
}