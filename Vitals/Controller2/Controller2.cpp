#pragma once

#define WIN32_LEAN_AND_MEAN

#include "ViewAdapter.h"
#include "VitalsModel.h"
#include "gui.h"
int main(int argc, char *argv[])
{
	GUI g = GUI();

	ViewAdapter vAdapt = ViewAdapter(g);

	boost::asio::io_service io;

	VitalsModel model(io, 20, 3, 750);

	model.start();
	io.run();

	model.setAdapter(vAdapt);

	g.start(argc, argv);
	return 0;
}