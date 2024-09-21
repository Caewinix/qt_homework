#include <QDateTime>
#include <QMessageBox>
#include "waveform_window.h"
#include "ui_waveform_window.h"
#include "users_manager.h"
#include <QDebug>

WaveformWindow::WaveformWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveformWindow)
{
    ui->setupUi(this);

    _plot=new HumanPlot("Numerical");
    ui->plotLay->addWidget(_plot);
}

void WaveformWindow::initPlot()
{
    _plot->graph(0)->data().data()->clear();
    _plot->graph(1)->data().data()->clear();
    QVector<double> timeData, oximetryData, temperatureData;
    _db->searchUserData(UsersManager::current(),[&](const Oximetry& oximetry,const Temperature& temperature,const QDateTime& saveTime){
//        qDebug()<<saveTime.toMSecsSinceEpoch()<<QTime::currentTime().msecsSinceStartOfDay()*0.001;
        timeData.append(saveTime.toMSecsSinceEpoch()*1e-3);
        oximetryData.append(oximetry);
        temperatureData.append(temperature);
    },"*");
    if(!timeData.isEmpty()){
        ui->lineEditFrom->setText(QDateTime::fromMSecsSinceEpoch(timeData.first()*1e3).toString("yyyy-MM-dd hh:mm:ss.zzz"));
        ui->lineEditTo->setText(QDateTime::fromMSecsSinceEpoch(timeData.last()*1e3).toString("yyyy-MM-dd hh:mm:ss.zzz"));
        _plot->plotData(timeData,oximetryData,temperatureData);
    }
}

WaveformWindow::~WaveformWindow()
{
    delete ui;
    delete _plot;
}



void WaveformWindow::on_btnOk_clicked()
{
    if(QDateTime::fromString(ui->lineEditFrom->text(),"yyyy-MM-dd hh:mm:ss.zzz").isValid()
            &&QDateTime::fromString(ui->lineEditTo->text(),"yyyy-MM-dd hh:mm:ss.zzz").isValid())
    {
        QVector<double> timeData, oximetryData, temperatureData;
        _db->searchUserData(UsersManager::current(),[&](const Oximetry& oximetry,const Temperature& temperature,const QDateTime& saveTime){
            qDebug()<<oximetry<<", "<<temperature<<", "<<saveTime;
            timeData.append(saveTime.toMSecsSinceEpoch()*1e-3);
            oximetryData.append(oximetry);
            temperatureData.append(temperature);
        },"*",QString("saveTime>\'%1\' AND saveTime<\'%2\'")
        .arg(ui->lineEditFrom->text(),
             ui->lineEditTo->text()));
        _plot->plotData(timeData,oximetryData,temperatureData);
    }else{
        QMessageBox::critical(this,"Error","Please follow the \"yyyy-MM-dd hh:mm:ss.zzz\" format!");
    }
}

