
#define WIN32_LEAN_AND_MEAN
#include "GUI/mainwindow.h"
#include "Model/VitalsModel.h"
#include <QtWidgets/QApplication>
#include <boost/thread/thread.hpp>


void startModel(boost::asio::io_service *io) {

	Serial *SP = new Serial(L"\\\\.\\COM16");    // adjust as needed
	if (SP->IsConnected())
		qDebug() << "We are connected\n";	
	else
		qDebug() << "We are not connected\n";	
	
	VitalsModel model(*io, SP, CameraParameters(Point(74,74), 240, 320, 15.24, 106.68, 1000, Point(0,20)), 20, 2, 750);

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