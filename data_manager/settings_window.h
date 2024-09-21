#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <QWidget>
#include "tcp_server.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

    bool isListening(){
        return _isListening;
    }

private:
    Ui::SettingsWindow *ui;
    TcpServer* _tcpServer;

    bool _isListening=false;
signals:
    /// A custom singal that will be emitted, when read the data.
    void read(Sender& sender, const Data&);
private slots:
    /// When clicked the button "Connect", it will be called.
    void _startListening();

    /// When clicked the button "Disconnect" it will be called.
    void _stopListening();

    /// When the data is sent to the client, it will be called.
    void _onRead(Sender& sender, const Data&);
    void on_btnLogout_clicked();
};

#endif // SETTINGS_WINDOW_H
