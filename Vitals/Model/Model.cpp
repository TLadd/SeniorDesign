// Model.cpp : Defines the entry point for the console application.
//

#include "VitalsModel.h"

int main( int argc, char** argv )
{
	boost::asio::io_service io;
	VitalsModel model(io, 20, 3, 750);
	model.start();
	io.run();

	return 0;
}

