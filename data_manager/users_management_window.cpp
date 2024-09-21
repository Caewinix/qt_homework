#include <QListWidgetItem>
#include <QMessageBox>
#include "users_management_window.h"
#include "user_list_item.h"
#include "ui_users_management_window.h"
#include <QDebug>

UsersManagementWindow::UsersManagementWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsersManagementWindow)
{
    _db=&ManagerDatabase::instance();

    ui->setupUi(this);

    fillList();
}

UsersManagementWindow::~UsersManagementWindow()
{
    delete ui;
}

void UsersManagementWindow::fillList(){
    _db->selectUsernames([&](const QString& username,bool isAdministrator){
        addItem(username,isAdministrator);
    });
}

void UsersManagementWindow::addItem(const QString& username,bool isAdministrator){
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    UserListItem* innerWidget=new UserListItem(username,isAdministrator,ui->listWidget);
    QSize size=innerWidget->sizeHint();
    size.setHeight(size.height()*1.1);
    item->setSizeHint(size);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->setItemWidget(item,innerWidget);
//    ui->listWidget->setCurrentItem(item);
//    qDebug()<<"row: "<<ui->listWidget->currentRow()<<", index: "<<ui->listWidget->currentIndex();
}

void UsersManagementWindow::on_btnAdd_clicked()
{
    if(_userAdderWnd!=nullptr){
        delete _userAdderWnd;
    }
    _userAdderWnd=new UserAdderWindow(this);
    _userAdderWnd->setWindowFlags(_userAdderWnd->windowFlags() & ~Qt::WindowMaximizeButtonHint);

    _userAdderWnd->cbxAuthority()->setCurrentIndex(0);

    connect(_userAdderWnd->btnAdd(),&QPushButton::clicked,this,[&](){
        const QString& username=_userAdderWnd->lineEditUsername()->text();
        const QString& password=_userAdderWnd->lineEditPassword()->text();
        bool isAdministrator=_userAdderWnd->cbxAuthority()->currentIndex();

        switch (ManagerDatabase::instance().signup(username,password,isAdministrator)) {
            case ManagerDatabase::signupSuccessful:
                {
                    QMessageBox::information(this,"Done","Signup seccessfully!");
                    addItem(username,isAdministrator);
                    _userAdderWnd->close();
                }
                break;
            case ManagerDatabase::signupFailed:
                QMessageBox::critical(this,"Error","Signup failed!");
                break;
            case ManagerDatabase::signupUsernameExisted:
                QMessageBox::critical(this,"Error","The username is already existed!\n");
                break;
        }
    });
    _userAdderWnd->show();

}
