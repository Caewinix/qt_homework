#include "human_plot.h"
#include "manager_database.h"
#include "users_manager.h"
#include <QDebug>

/// This function initializes the line chart widgets and sets their colors, formats, and ranges (Juanhua Zhang)
HumanPlot::HumanPlot(const char* chartName)
{
     legend->setVisible(true);

//     legend->setSelectableParts(QCPLegend::spItems);
//     setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//                                                   //Turn on the mouse drag, scroll wheel zoom function
     yAxis->setLabel(chartName);


     addGraph();
     //Adding the first curve
     graph(0)->setName("Oximetry (%)");
     addGraph();
     //Adding the second curve
     graph(1)->setName("Temperature (℃)");
}

///// This function is used to update the line chart in real time (Juanhua Zhang)
//void HumanPlot::addData(QDateTime time,double oximetry, double temperature){
//    qint64 timestamp=time.currentMSecsSinceEpoch();
//     graph(0)->addData(timestamp,oximetry); // Add data to the first day curve
//     graph(1)->addData(timestamp,temperature);

//     qDebug()<<timestamp<<' '<<oximetry<<' '<<temperature<<'\n';

//     xAxis->setRange(timestamp, timestamp+0, Qt::AlignRight); // Set the x-axis range

//     replot(); // Refresh the curve
////     ++timeKey;
//}

//void HumanPlot::clear(){
//    graph(0)->c
//}

void HumanPlot::plotData(const QVector<double>& timeData,const QVector<double>& oximetryData,const QVector<double>& temperatureData)
{

    graph(0)->setData(timeData, oximetryData);
    graph(1)->setData(timeData, temperatureData);
    QPen pen;
    pen.setWidth(2); // sets the line width to 2
    graph(0)->setPen(pen);
        graph(1)->setPen(pen);

        graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
        graph(1)->setPen(QPen(Qt::red)); // line color red for second graph

        // Set axes labels
        xAxis->setLabel("Time");
        yAxis->setLabel("Oximetry (%)");

    //    // 创建右侧y轴
        yAxis2->setVisible(true);
        yAxis2->setVisible(true);

        xAxis2->setLabel("Time");
        yAxis2->setLabel("Temperature (℃)");


         graph(0)->rescaleAxes();
         graph(1)->rescaleAxes(true);


        yAxis->setTickLabels(true);
        yAxis2->setTickLabels(true);


        graph(0)->setValueAxis(yAxis);
        graph(1)->setValueAxis(yAxis2);


        graph(0)->setLineStyle(QCPGraph::lsLine);
        graph(1)->setLineStyle(QCPGraph::lsLine);

        setAntialiasedElements(QCP::aeAll);

        QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
        timeTicker->setDateTimeFormat("yyyy-MM-dd\nhh:mm:ss"); // Set the time format
         xAxis->setTicker(timeTicker); // Set the x-axis display format

        yAxis->setRange(80, 100);
       yAxis2->setRange(30, 50);

        replot();

}

///// This function is used to update the line chart in real time and make real-time statistics on the data (Juanhua Zhang)
//void HumanStatistics::addData(QDateTime time,double oximetry, double temperature){
//    if(oximetry > maxOximetry){
//        maxOximetry = oximetry;
//    }
//    else if(oximetry < minOximetry){
//        minOximetry = oximetry;
//    }

//    if(temperature > maxTemperature){
//        maxTemperature = temperature;
//    }
//    else if(temperature < minTemperature){
//        minTemperature = temperature;
//    }
//    // Statistics of data

//    count++;
//    temperatureTotal += temperature;
//    oximetryTotal += oximetry;

//    HumanPlot::addData(time,oximetry,temperature);
//}
