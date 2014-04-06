/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011, 2012, 2013 Emanuel Eichhammer                     **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 09.12.13                                             **
**          Version: 1.1.1                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

#include "../Model/ViewAdapter.h"
 

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setGeometry(100, 100, 600, 600);
  setupDemo();
  //setupPlayground(ui->customPlot);
  // 0:  setupQuadraticDemo(ui->customPlot);
  // 1:  setupSimpleDemo(ui->customPlot);
  // 2:  setupSincScatterDemo(ui->customPlot);
  // 3:  setupScatterStyleDemo(ui->customPlot);
  // 4:  setupScatterPixmapDemo(ui->customPlot);
  // 5:  setupLineStyleDemo(ui->customPlot);
  // 6:  setupDateDemo(ui->customPlot);
  // 7:  setupTextureBrushDemo(ui->customPlot);
  // 8:  setupMultiAxisDemo(ui->customPlot);
  // 9:  setupLogarithmicDemo(ui->customPlot);
  // 10: setupRealtimeDataDemo(ui->customPlot);
  // 11: setupParametricCurveDemo(ui->customPlot);
  // 12: setupBarChartDemo(ui->customPlot);
  // 13: setupStatisticalDemo(ui->customPlot);
  // 14: setupSimpleItemDemo(ui->customPlot);
  // 15: setupItemDemo(ui->customPlot);
  // 16: setupStyledDemo(ui->customPlot);
  // 17: setupAdvancedAxesDemo(ui->customPlot);
  
  // for making screenshots of the current demo or all demos (for website screenshots):
  //QTimer::singleShot(1500, this, SLOT(allScreenShots()));
  //QTimer::singleShot(1000, this, SLOT(screenShot()));
}

void MainWindow::setupDemo()
{
  setupRealtimeDataDemo();
  setWindowTitle("QCustomPlot: "+demoName);
  statusBar()->clearMessage();
  ui->heartPlot->yAxis->setRange(-1, 1);
  ui->heartPlot->replot();
}


void MainWindow::setupRealtimeDataDemo()
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
  demoName = "Real Time Data Demo";
  
  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  //HEART RATE:
  // blue line
  ui->heartPlot->addGraph();
  ui->heartPlot->graph(0)->setPen(QPen(Qt::green));
  ui->heartPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  ui->heartPlot->graph(0)->setAntialiasedFill(false);
  // blue dot
  ui->heartPlot->addGraph();
  ui->heartPlot->graph(1)->setPen(QPen(Qt::green));
  ui->heartPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  ui->heartPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
  //axis
  ui->heartPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->heartPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  ui->heartPlot->xAxis->setAutoTickStep(false);
  ui->heartPlot->xAxis->setTickStep(2);
  ui->heartPlot->axisRect()->setupFullAxesBox();
  ui->heartPlot->yAxis->setRange(-1, 1);
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(ui->heartPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->heartPlot->xAxis2, SLOT(setRange(QCPRange)));
  //connect(ui->heartPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->heartPlot->yAxis2, SLOT(setRange(QCPRange)));

  //BREATHING RATE:
  // blue line
  ui->breathingPlot->addGraph();
  ui->breathingPlot->graph(0)->setPen(QPen(Qt::blue));
  ui->breathingPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  ui->breathingPlot->graph(0)->setAntialiasedFill(false);
  // blue dot
  ui->breathingPlot->addGraph();
  ui->breathingPlot->graph(1)->setPen(QPen(Qt::blue));
  ui->breathingPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  ui->breathingPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
  //axis
  ui->breathingPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->breathingPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  ui->breathingPlot->xAxis->setAutoTickStep(false);
  ui->breathingPlot->xAxis->setTickStep(2);
  ui->breathingPlot->axisRect()->setupFullAxesBox();
  ui->breathingPlot->yAxis->setRange(-1, 1);
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(ui->breathingPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->breathingPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->breathingPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->breathingPlot->yAxis2, SLOT(setRange(QCPRange)));


  //TEMPERATURE PLOT
  //BREATHING RATE:
  // blue line
  ui->temperaturePlot->addGraph();
  ui->temperaturePlot->graph(0)->setPen(QPen(Qt::red));
 // ui->temperaturePlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  ui->temperaturePlot->graph(0)->setAntialiasedFill(false);
  // blue dot
  ui->temperaturePlot->addGraph();
  ui->temperaturePlot->graph(1)->setPen(QPen(Qt::red));
  ui->temperaturePlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  ui->temperaturePlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
  //axis
  ui->temperaturePlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->temperaturePlot->xAxis->setDateTimeFormat("hh:mm:ss");
  ui->temperaturePlot->xAxis->setAutoTickStep(false);
  ui->temperaturePlot->xAxis->setTickStep(2);
  ui->temperaturePlot->axisRect()->setupFullAxesBox();
  ui->temperaturePlot->yAxis->setRange(0, 50);
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(ui->temperaturePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->temperaturePlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->temperaturePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->temperaturePlot->yAxis2, SLOT(setRange(QCPRange)));


  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(500); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot()
{
  // calculate two new data points:
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  double key = 0;
#else
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
#endif
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.01) // at most add point every 10 ms
  {
    double heartVal = qSin(key); //sin(key*1.6+cos(key*1.7)*2)*10 + sin(key*1.2+0.56)*20 + 26;
    double breathingVal = qCos(key); //sin(key*1.3+cos(key*1.2)*1.2)*7 + sin(key*0.9+0.26)*24 + 26;
    double temperatureVal = 0.5*qCos(key) + 0.5*qSin(key); //sin(key*1.3+cos(key*1.2)*1.2)*7 + sin(key*0.9+0.26)*24 + 26;

    //HEART RATE
    //display value
  //  ui->heartRateVal->display(100*qSin(key / 100));
  //  ui->heartPlot->graph(0)->addData(key, heartVal);
    // set data of dots:
  //  ui->heartPlot->graph(1)->clearData();
  //  ui->heartPlot->graph(1)->addData(key, heartVal);
    // remove data of lines that's outside visible range:
  //  ui->heartPlot->graph(0)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
  //  ui->heartPlot->graph(0)->rescaleValueAxis();

    //BREATHING RATE
    //display value
   // ui->breathingRateVal->display(100*qSin(key / 500));
   // ui->breathingPlot->graph(0)->addData(key, breathingVal);
    // set data of dots:
   // ui->breathingPlot->graph(1)->clearData();
   // ui->breathingPlot->graph(1)->addData(key, breathingVal);
    // remove data of lines that's outside visible range:
   // ui->breathingPlot->graph(0)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
   // ui->breathingPlot->graph(0)->rescaleValueAxis();

    //TEMPERATURE
    //display value
    //ui->temperatureVal->display(100*qSin(key / 1200));
   // ui->temperaturePlot->graph(0)->addData(key, temperatureVal);
    // set data of dots:
  //  ui->temperaturePlot->graph(1)->clearData();
   // ui->temperaturePlot->graph(1)->addData(key, temperatureVal);
    // remove data of lines that's outside visible range:
  //  ui->temperaturePlot->graph(0)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
   // ui->temperaturePlot->graph(0)->rescaleValueAxis();



    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
//  ui->heartPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
//  ui->heartPlot->yAxis->setRange(-1, 1);
//  ui->heartPlot->replot();
//  ui->breathingPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
//  ui->breathingPlot->yAxis->setRange(-1, 1);
//  ui->breathingPlot->replot();
  boost::mutex::scoped_lock lock(guard);
  ui->temperaturePlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
  //ui->temperaturePlot->yAxis->setRange(10, 40);
  ui->temperaturePlot->replot();
  
  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->heartPlot->graph(0)->data()->count()+ui->heartPlot->graph(1)->data()->count())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}

void MainWindow::setupPlayground(QCustomPlot *customPlot)
{
  Q_UNUSED(customPlot)
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::screenShot()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
#else
  QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
#endif
  QString fileName = demoName.toLower()+".png";
  fileName.replace(" ", "");
  pm.save("./screenshots/"+fileName);
  qApp->quit();
}

void MainWindow::setTemperatureValue(double newVal){
	boost::mutex::scoped_lock lock(guard);
	qDebug() << "In the Viewwwww" << endl;
	ui->temperatureVal->display(newVal);
}

void MainWindow::updateTemperatureGraph(double newVal){
	boost::mutex::scoped_lock lock(guard);
    //ui->temperatureVal->display(100*qSin(key / 1200));
	qDebug() << "adding to temperature graph in view" << endl;
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    ui->temperaturePlot->graph(0)->addData(key, newVal);
    // set data of dots:
    ui->temperaturePlot->graph(1)->clearData();
    ui->temperaturePlot->graph(1)->addData(key, newVal);
    // remove data of lines that's outside visible range:
    ui->temperaturePlot->graph(0)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->temperaturePlot->graph(0)->rescaleValueAxis();

//	ui->temperaturePlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
//	ui->temperaturePlot->yAxis->setRange(5, 45);
//	ui->temperaturePlot->replot();
}

void MainWindow::setBreathingRateValue(double newVal){
	boost::mutex::scoped_lock lock(guard);
	qDebug() << "In the Viewwwww" << endl;
	ui->breathingRateVal->display(newVal);
}

void MainWindow::updateBreathingRateGraph(double newVal){
	boost::mutex::scoped_lock lock(guard);
    //ui->temperatureVal->display(100*qSin(key / 1200));
	qDebug() << "adding to breathing graph in view" << endl;
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    ui->breathingPlot->graph(0)->addData(key, newVal);
    // set data of dots:
    ui->breathingPlot->graph(1)->clearData();
    ui->breathingPlot->graph(1)->addData(key, newVal);
    // remove data of lines that's outside visible range:
    ui->breathingPlot->graph(0)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->breathingPlot->graph(0)->rescaleValueAxis();

	ui->breathingPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
	//ui->breathingPlot->yAxis->setRange(5, 45);
	ui->breathingPlot->replot();
}

void MainWindow::setHeartRateValue(double newVal){
	boost::mutex::scoped_lock lock(guard);
	qDebug() << "In the Viewwwww" << endl;
	ui->heartRateVal->display(newVal);
}

void MainWindow::updateHeartRateGraph(double newVal){
    //ui->temperatureVal->display(100*qSin(key / 1200));
	boost::mutex::scoped_lock lock(guard);
	qDebug() << "adding to breathing graph in view" << endl;
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    ui->heartPlot->graph(0)->addData(key, newVal);
    // set data of dots:
    ui->heartPlot->graph(1)->clearData();
    ui->heartPlot->graph(1)->addData(key, newVal);
    // remove data of lines that's outside visible range:
    ui->heartPlot->graph(0)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->heartPlot->graph(0)->rescaleValueAxis();

	ui->heartPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
	//ui->heartPlot->yAxis->setRange(5, 45);
	ui->heartPlot->replot();
}

void MainWindow::setHeartRateGraph(std::vector<int> intKeyData, std::vector<float> floatValData){
    //ui->temperatureVal->display(100*qSin(key / 1200));
	boost::mutex::scoped_lock lock(guard);
	qDebug() << "setting heartrateGraph in view" << endl;

	std::vector<double> const doubleKeyData(intKeyData.begin(), intKeyData.end());
	std::vector<double> const doubleValData(floatValData.begin(), floatValData.end());

	QVector<double> keyQVector = QVector<double>::fromStdVector( doubleKeyData );
	QVector<double> valQVector = QVector<double>::fromStdVector( doubleValData );

	double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    ui->heartPlot->graph(0)->setData(keyQVector, valQVector);
    // set data of dots:
    //ui->heartPlot->graph(1)->clearData();
    //ui->heartPlot->graph(1)->addData(key, newVal);
    // remove data of lines that's outside visible range:
    //ui->heartPlot->graph(0)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    //ui->heartPlot->graph(0)->rescaleValueAxis();

	//ui->heartPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
	ui->heartPlot->yAxis->setRange(5, 45);
	ui->heartPlot->replot();
}

//make all calls to adapter
void MainWindow::configureViewAdapter(ViewAdapter* adap){
	boost::mutex::scoped_lock lock(guard);
	conn_setTemperatureVal = adap->connect_setTemperatureVal(boost::bind(&MainWindow::setTemperatureValue, this, _1));
	conn_updateTemperatureGraph = adap->connect_updateTemperatureGraph(boost::bind(&MainWindow::updateTemperatureGraph, this, _1));

	conn_setHeartRateVal = adap->connect_setHeartRateVal(boost::bind(&MainWindow::setHeartRateValue, this, _1));
	conn_updateHeartRateGraph = adap->connect_updateHeartRateGraph(boost::bind(&MainWindow::updateHeartRateGraph, this, _1));
	conn_setHeartRateGraph = adap->connect_setHeartRateGraph(boost::bind(&MainWindow::setHeartRateGraph, this, _1, _2));

	conn_setBreathingRateVal = adap->connect_setBreathingRateVal(boost::bind(&MainWindow::setBreathingRateValue, this, _1));
	conn_updateBreathingRateGraph = adap->connect_updateBreathingRateGraph(boost::bind(&MainWindow::updateBreathingRateGraph, this, _1));
}







































