#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

#include "users_management_window.h"
#include "waveform_window.h"
#include "data_window.h"
#include "settings_window.h"
#include "users_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow& instance(QWidget* parent = nullptr);
    UsersManagementWindow& usersManagementWindow(){
        return usersManagementWnd;
    }
    WaveformWindow& waveformWindow(){
        return waveformWnd;
    }
    DataWindow& dataWindow(){
        return dataWnd;
    }
    ~MainWindow();

    void show();

signals:
    void updateCurrentUser();

private:
    MainWindow(QWidget *parent = nullptr);

    static MainWindow* _instance;

    Ui::MainWindow *ui;

    QButtonGroup btnGroup;
    UsersManagementWindow usersManagementWnd;
    WaveformWindow waveformWnd;
    DataWindow dataWnd;
    SettingsWindow settingsWnd;

private slots:
    void _changeCurrentUsername();
};
#endif // MAIN_WINDOW_H
