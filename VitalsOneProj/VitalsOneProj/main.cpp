
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
	
	CameraParameters cp(Point(74,74), 240, 320, 15.24, 106.68, 255, Point(0,10));
	GimbalHelper gh(cp, SP);

	/*
	int ys[8] = { 130, 130, 130}; 
	int xs[8] = { 60, 260, 160}; 
	int index = 0;
	while(true) {
		
		gh.positionGimbal(Point(xs[index%3], ys[index%3]), 320);
		Sleep(5000);
		index++;
	} 
	*/

	VitalsModel model(*io, SP, cp, 20, 2, 200);

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