#ifndef WAVEFORM_WINDOW_H
#define WAVEFORM_WINDOW_H

#include <QWidget>
#include "human_plot.h"
#include "manager_database.h"

namespace Ui {
class WaveformWindow;
}

class WaveformWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WaveformWindow(QWidget *parent = nullptr);
    ~WaveformWindow();

    void plotData();
    void initPlot();

private slots:
    void on_btnOk_clicked();

private:

    void _initDatabase(){
        _db=&ManagerDatabase::instance();
    }

    Ui::WaveformWindow *ui;
    HumanPlot* _plot; // Painting class
    ManagerDatabase* _db;
};

#endif // WAVEFORM_WINDOW_H
