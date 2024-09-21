#include <QMessageBox>
#include "settings_window.h"
#include "ui_settings_window.h"
#include "manager_database.h"
#include "main_window.h"
#include <QDebug>
#include <QHostInfo>
#include <QJsonDocument>
#include <QJsonObject>

//void _ipAddress(){
//    QList<QNetworkInterface> list=QNetworkInterface::allInterfaces();//获取网络接口

//    for(int i=0;i<list.count();i++)

//    {

//    QNetworkInterface aInterface=list.at(i);

//    qDebug()<<"设备名称:"<<aInterface.humanReadableName(); //显示每个接口的设备名称

//    qDebug()<<"硬件地址:"<<aInterface.hardwareAddress(); //显示每个接口的硬件地址

//    QList<QNetworkAddressEntry> entryList=aInterface.addressEntries();

//    for(int j=0;j<entryList.count();j++)

//    {

//    QNetworkAddressEntry aEntry=entryList.at(j);

//    qDebug()<<"IP地址："+aEntry.ip().toString();

//    }

//    }
//}

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    _tcpServer=&TcpServer::instance();

    ui->setupUi(this);

    ui->lineEditIpAddress->setText("127.0.0.1");
    ui->lineEditPort->setText("10008");
//    _ipAddress();

    connect(ui->btnConnectOrNot,&QPushButton::clicked,this,&SettingsWindow::_startListening);

    connect(this,&SettingsWindow::read,this,&SettingsWindow::_onRead);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::_startListening(){
    if(_isListening){
        _stopListening();
    }else{
        const QString& ipAddress=ui->lineEditIpAddress->text().trimmed();
        const QString& portStr=ui->lineEditPort->text().trimmed();
        if(ipAddress!=""&&portStr!=""){
            // When the IP address and port are entered correctly.
            _tcpServer->startListening(QHostAddress(ipAddress),portStr.toUInt(),
               [&](const PeerAddress&, const PeerPort&, Sender& sender, const Data& data){
                // emit the cunstom signal.
                emit read(sender,data);
            });
            if(_tcpServer->isListening()){
                // When it connected successfully.
                _isListening=true;
                ui->btnConnectOrNot->setText("Disconnect");
            }
            else
                QMessageBox::critical(this,"Error","Failed to listen.");
        }else{
            QMessageBox::warning(this,"Warning","Please enter the IP Address or Port!");
        }
    }
}

void SettingsWindow::_stopListening(){
    _tcpServer->close();
    _isListening=false;
    ui->btnConnectOrNot->setText("Connect");
}

//void SettingsWindow::_onPressedSend(){

//    _tcpServer->sendAll(serializeCustomData("Server",msg,"orange",5));


//}

void SettingsWindow::_onRead(Sender& sender, const Data& data){
    qDebug()<<data;
    QJsonDocument responseDoc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject responseObj = responseDoc.object();
    const QString type=responseObj["type"].toString();
    if(type=="data"){
        QString username = responseObj["username"].toString();
        double temperature = responseObj["temperature"].toDouble();
        double oximetry = responseObj["oximetry"].toDouble();
        ManagerDatabase::instance().insertUserData(username,oximetry,temperature);
    }else if(type=="login"){
        QString username = responseObj["username"].toString();
        QString password = responseObj["password"].toString();
        QJsonObject jsonObject;
        switch(ManagerDatabase::instance().login(username,password,true)){
            case ManagerDatabase::loginSuccessful:
                {
                    jsonObject["type"] = "login";
                    jsonObject["username"] = username;
                    jsonObject["success"] = true;
                }
                break;
            case ManagerDatabase::loginPasswordMismatch:
                {
                    jsonObject["type"] = "login";
                    jsonObject["username"] = username;
                    jsonObject["success"] = false;
                    jsonObject["reason"]="password mismatch";
                }
                break;
            case ManagerDatabase::loginUserNameNotFound:
                {
                    jsonObject["type"] = "login";
                    jsonObject["username"] = username;
                    jsonObject["success"] = false;
                    jsonObject["reason"]="username not found";
                }
                break;
            case ManagerDatabase::loginOvertime:
                {
                    jsonObject["type"] = "login";
                    jsonObject["username"] = username;
                    jsonObject["success"] = false;
                    jsonObject["reason"]="login overtime";
                }
                break;
            default:break;
        }
        QJsonDocument jsonDoc(jsonObject);
        QByteArray jsonData = jsonDoc.toJson();
        _tcpServer->sendTo(sender,jsonData);
    }else if(type=="signup"){
        QString username = responseObj["username"].toString();
        QString password = responseObj["password"].toString();
        QJsonObject jsonObject;
        switch (ManagerDatabase::instance().signup(username,password,false)) {
            case ManagerDatabase::signupSuccessful:
                {
                    jsonObject["type"] = "signup";
                    jsonObject["username"] = username;
                    jsonObject["success"] = true;
                    MainWindow::instance().usersManagementWindow().addItem(username,false);
                }
                break;
            case ManagerDatabase::signupFailed:
                {
                    jsonObject["type"] = "signup";
                    jsonObject["username"] = username;
                    jsonObject["success"] = false;
                    jsonObject["reason"]="failed";
                }
                break;
            case ManagerDatabase::signupUsernameExisted:
                {
                    jsonObject["type"] = "signup";
                    jsonObject["username"] = username;
                    jsonObject["success"] = false;
                    jsonObject["reason"]="username existed";
                }
                break;
        }
        QJsonDocument jsonDoc(jsonObject);
        QByteArray jsonData = jsonDoc.toJson();
        _tcpServer->sendTo(sender,jsonData);
    }
}

void SettingsWindow::on_btnLogout_clicked()
{
    MainWindow::instance().close();
}

