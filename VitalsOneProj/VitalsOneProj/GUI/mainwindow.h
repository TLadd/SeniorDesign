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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h" // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.
#include <boost/signals2/signal.hpp>
class ViewAdapter;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
  void setupDemo();
  void setupRealtimeDataDemo(/*QCustomPlot *customPlot, QCustomPlot *customPlot_2*/);
  
  void setupPlayground(QCustomPlot *customPlot);

  void setTemperatureValue(double newVal);
  void updateTemperatureGraph(double newVal);
  
  void setBreathingRateValue(double newVal);
  void updateBreathingRateGraph(double newVal);
  
  void setHeartRateValue(double newVal);
  void updateHeartRateGraph(double newVal);
  
  // adapter interface setup
  void configureViewAdapter(ViewAdapter* adap);
  
private slots:
  void realtimeDataSlot();
  void screenShot();


  // functions to create conections to ViewAdater methods
 // void set_conn_setTemperatureValue(ViewAdapter& adap);
  
private:
  Ui::MainWindow *ui;
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;

  // connections to subscribe to ViewAdapter methods
  boost::signals2::connection conn_setTemperatureVal;
  boost::signals2::connection conn_updateTemperatureGraph;

  boost::signals2::connection conn_setBreathingRateVal;
  boost::signals2::connection conn_updateBreathingRateGraph;

  boost::signals2::connection conn_setHeartRateVal;
  boost::signals2::connection conn_updateHeartRateGraph;
};

#endif // MAINWINDOW_H
