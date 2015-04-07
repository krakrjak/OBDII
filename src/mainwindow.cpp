#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "qcustomplot.h"

QVector<double> a(101), b(101);
QVector<double> c(101), d(101);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->conn = new serial::PortReaderWriter();
    connect();

    setWindowTitle("Group 2 Software Engineering ODBII Reader");
    speedCount = 1;
    rpmCount = 1;
    a[0] = 0;
    b[0] = 0;
    c[0] = 0;
    d[0] = 0;

//    setupQuadraticDemo(ui->customPlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupQuadraticDemo(QCustomPlot *customPlot)
{
  //demoName = "Quadratic Demo";
  // generate some data:
  QVector<double> x(101), y(101); // initialize with entries 0..100
//  for (int i=0; i<101; ++i)
  for (int i=0; i<10; i++)
  {
      x[i] = i;
      switch (i){
      case 0: y[i] = 0; break;
      case 1: y[i] = 0; break;
      case 2: y[i] = 1; break;
      case 3: y[i] = 2; break;
      case 4: y[i] = 4; break;
      case 5: y[i] = 6; break;
      case 6: y[i] = 5; break;
      case 7: y[i] = 4; break;
      case 8: y[i] = 1; break;
      case 9: y[i] = 0; break;
      }

  //  x[i] = i/50.0 - 1; // x goes from -1 to 1
  //  y[i] = x[i]*x[i];  // let's plot a quadratic function
  }
//  y[11] = {0,0,1,2,4,6,5,6,4,3,0};
  // create graph and assign data to it:
  customPlot->addGraph();
  customPlot->graph(0)->setData(x, y);
  // give the axes some labels:
  customPlot->xAxis->setLabel("Time");
  customPlot->yAxis->setLabel("Value");
  // set axes ranges, so we see all data:
  customPlot->xAxis->setRange(0, 10);
  customPlot->yAxis->setRange(0, 7);
  customPlot->graph(0)->setPen(QPen(Qt::red)); // line color blue for first graph
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
}

void MainWindow::on_pushButton_clicked()
{
    QString trobelCode = "";
    { // Try to get the Trouble Code
        conn->sendCommand("01 01");
        // TODO: Needs to be multiline aware
        QByteArray buff = conn->readLine();
        trobelCode = conn->decodeErr(buff);
        ui->textBrowser->setText(trobelCode);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    int vehicleSpeed = 0;
   { // Try to get the RPM
       conn->sendCommand("01 0D");
       QByteArray buff = conn->readLine();
       vehicleSpeed = conn->decodeVehicleSpeed(buff);
   }
   ui->textBrowser->setText(  QString::number(vehicleSpeed)  );
   d[speedCount] = vehicleSpeed;
   speedCount++;
}

void MainWindow::on_pushButton_3_clicked()
{
    int rpmVal = 0;
    { // Try to get the RPM
        conn->sendCommand("01 0C");
        QByteArray buff = conn->readLine();
        rpmVal = conn->decodeRPM(buff);
    }
    ui->textBrowser->setText( QString::number(rpmVal) );
    b[rpmCount] = rpmVal/1000;
    rpmCount++;
}

void MainWindow::on_pushButton_4_clicked()
{
    sendcommand();
}

void MainWindow::on_radioButton_clicked()
{
    setupSpeedGraph(ui->customPlot);
}

void MainWindow::on_radioButton_2_clicked()
{
    setupRPMGraph(ui->customPlot);
}

void MainWindow::connect()
{
    QString infoout="";

    if (!conn->serialConnect())
    {
        ui->textBrowser->setText("Could not connect!");
    }


    if (this->conn->isConnected())
    {
        infoout = "Connected to serial port!" +  this->conn->getConnectedPortName();
        ui->textBrowser->setText(infoout);
    }
    else
    {
        ui->textBrowser->setText("Not connected to a serial port!");
    }

}

void MainWindow::sendcommand()
{

    QString tempstr="";

    QString instr = ui->lineEdit->text();
    QByteArray qbin = instr.toUtf8();

    if(!conn->sendCommand( qbin ))
    {
        ui->textBrowser->append( "Problem writing !!!!");
    }
    QByteArray buff = conn->readLine();
    tempstr = "Buff size 1: ";
    tempstr += QString::number(buff.size());
    ui->textBrowser->append( tempstr );

    buff.remove(0, instr.size()+1);

    tempstr = "Buff size 2: ";
    tempstr += QString::number( buff.size() );

    ui->textBrowser->append( tempstr );

    tempstr = "(";
    tempstr += buff;
    tempstr += ")" ;

    ui->textBrowser->append( tempstr );

    ui->lineEdit->setText("");
}

void MainWindow::setupSpeedGraph(QCustomPlot *customPlot)
{
  for (int i=0; i<=speedCount; i++)
  {
      c[i] = i;
  }
  // create graph and assign data to it:
  //ui->customPlot->replot();
  customPlot->addGraph();
  customPlot->graph(0)->setData(c, d);
  // give the axes some labels:
  customPlot->xAxis->setLabel("Count");
  customPlot->yAxis->setLabel("Speed (MPH)");
  // set axes ranges, so we see all data:
  customPlot->xAxis->setRange(0, speedCount - 1);
  customPlot->yAxis->setRange(-2, 80);
  customPlot->graph(0)->setPen(QPen(Qt::red)); // line color blue for first graph
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
  ui->customPlot->replot();
}

void MainWindow::setupRPMGraph(QCustomPlot *customPlot)
{
  for (int i=0; i<=speedCount; i++)
  {
      a[i] = i;
  }
  // create graph and assign data to it:
  //ui->customPlot->replot();
  customPlot->addGraph();
  customPlot->graph(0)->setData(a, b);
  // give the axes some labels:
  customPlot->xAxis->setLabel("Count");
  customPlot->yAxis->setLabel("RPM (x1000)");
  // set axes ranges, so we see all data:
  customPlot->xAxis->setRange(0, rpmCount - 1);
  customPlot->yAxis->setRange(-2, 10);
  customPlot->graph(0)->setPen(QPen(Qt::red)); // line color blue for first graph
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
  ui->customPlot->replot();
}

void MainWindow::on_pushButton_7_clicked()
{
    QFont font;
    font.setPointSize(12);
    ui->label->setText("Connection Status: (Dis)Connected");
    ui->label->setFont(font);
}