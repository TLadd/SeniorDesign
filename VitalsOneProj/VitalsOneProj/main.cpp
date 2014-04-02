
#define WIN32_LEAN_AND_MEAN
#include "GUI/mainwindow.h"
#include "Model/VitalsModel.h"
#include <QtWidgets/QApplication>
#include <boost/thread/thread.hpp>
#include <boost/signals2/signal.hpp>


struct HelloWorld
{
  void operator()() const
  {
    qDebug() << "Hello, World!";
  }
};

void startModel(boost::asio::io_service *io, ViewAdapter *adap) {

	Serial *SP = new Serial(L"\\\\.\\COM16");    // adjust as needed
	if (SP->IsConnected())
		qDebug() << "We are connected\n";	
	else
		qDebug() << "We are not connected\n";	
	
	CameraParameters cp(Point(74,74), 240, 320, 15.24, 106.68, 255, Point(0,10));
	GimbalHelper gh(cp, SP);


	VitalsModel model(*io, SP, cp, 20, 2, 200);
	model.setAdapter(adap);

	model.start();
	io->run();
}

unsigned long getThreadId(){
    std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
    unsigned long threadNumber = 0;
    sscanf(threadId.c_str(), "%lx", &threadNumber);
    return threadNumber;
}

void drawThreadId(){
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
}

void argTest(float arg){
	 qDebug() << getThreadId();
	qDebug() << "[" << arg << "]";
}




void threadTest(){
	boost::signals2::signal<void (float)> threadSignal;
	threadSignal.connect(&argTest);
	threadSignal(4.2069);
}

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	MainWindow w;
	ViewAdapter adap(w);
	w.configureViewAdapter(&adap);
	boost::asio::io_service io;
	thread *t = new thread(&startModel, &io, &adap);


	//boost::signals::signal<void()> mySignal;

	//MyClass myClass;
	//mySignal.connect(boost::bind(&MyClass::doSomething, boost::ref(myClass)));

	// launches a thread and executes myClass.loop() there
	//boost::thread t(boost::bind(&MyClass::loop(), boost::ref(myClass)));

	  // Signal with no arguments and a void return value
	  //boost::signals2::signal<void ()> sig;
	   //boost::signals2::signal<void (float)> sig2;

	  // Connect a HelloWorld slot
	  //HelloWorld hello;
	  //sig.connect(hello);
	  //sig2.connect(&argTest);
	  
	 // thread *t2 = new thread(&threadTest);

	  // Call all of the slots
	 // sig();
	 // sig2(1.23);

	w.show();
	return a.exec();
}