#include "main_window.h"
#include "ui_main_window.h"
#include "login_dialog.h"

MainWindow* MainWindow::_instance=nullptr;

MainWindow& MainWindow::instance(QWidget* parent){
    if(_instance==nullptr){
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(_instance==nullptr){
            _instance=new MainWindow(parent);
        }
    }
    return *_instance;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(&usersManagementWnd);
    ui->stackedWidget->addWidget(&waveformWnd);
    ui->stackedWidget->addWidget(&dataWnd);
    ui->stackedWidget->addWidget(&settingsWnd);

//    btnGroup.setExclusive(false);
    btnGroup.addButton(ui->btnUsersManagement, 0);
    btnGroup.addButton(ui->btnWaveform, 1);
    btnGroup.addButton(ui->btnData, 2);
    btnGroup.addButton(ui->btnSettings, 3);
    connect(&btnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    btnGroup.button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);

    connect(this,&MainWindow::updateCurrentUser,this,&MainWindow::_changeCurrentUsername);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show(){
    LoginDialog([&](const QString& username){
        QMainWindow::show();
        UsersManager::setCurrentUser(username);
        emit updateCurrentUser();
        ui->labelWelcome->setText(QString("Welcome,   %1!").arg(username));
    }).exec();
}

void MainWindow::_changeCurrentUsername(){
    ui->labelCurrentUsername->setText(QString("%1' s").arg(UsersManager::current()));
    dataWnd.initTableWidget();
    waveformWnd.initPlot();
}
