#ifndef HUMAN_LPLOT_H
#define HUMAN_LPLOT_H
#include "qcustomplot.h"
#include <cfloat>
#include <QDateTime>

/// This class is upgraded to a class based on qcustomplot.h, which draws line graphs in real time and makes statistics (Juanhua Zhang)
class HumanPlot:public QCustomPlot
{
public:
    HumanPlot(const char* chartName);
    void plotData(const QVector<double>& timeData,const QVector<double>& oximetryData,const QVector<double>& temperatureData);
};

///// This class is upgraded to a class based on qcustomplot.h, which draws line graphs in real time and makes statistics (Juanhua Zhang)
//class HumanStatistics:public HumanPlot
//{
//public:
//    HumanStatistics():HumanPlot("Numerical"){}
//    void plotData();
//    void addData(QDateTime time,double oximetry, double temperature);
//    double temperatureTotal = 0,oximetryTotal = 0;
//    double maxTemperature = DBL_MIN,minTemperature = DBL_MAX;
//    double maxOximetry = DBL_MIN,minOximetry = DBL_MAX;
//    quint64 count = 0;
//};

#endif // HUMAN_LPLOT_H
