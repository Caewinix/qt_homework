#include "user_list_item.h"
#include "manager_database.h"
#include "users_manager.h"
#include <QMessageBox>
#include <QDebug>

UserListItem::UserListItem(const QString& username,bool isAdministrator,QListWidget* listWidget,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListItem),
    _username(username),
    _isAdministrator(isAdministrator),
    _listWidget(listWidget)
{
    ui->setupUi(this);
    ui->labelUsername->setText(_username);

    updateUsernameColor();

    connect(ui->btnModify,&QPushButton::clicked,this,[&](){
        if(_userModifierWnd!=nullptr){
            delete _userModifierWnd;
        }
        _userModifierWnd=new UserModifierWindow(this);
        _userModifierWnd->setWindowFlags(_userModifierWnd->windowFlags() & ~Qt::WindowMaximizeButtonHint);

        _userModifierWnd->cbxAuthority()->setCurrentIndex((int)_isAdministrator);

        _userModifierWnd->usernameDisplay()->setText(_username);

        connect(_userModifierWnd->btnConfirm(),&QPushButton::clicked,this,[&](){
            const QString& newPassword=_userModifierWnd->lineEditPassword()->text();
            if(!newPassword.isEmpty()){
                if(ManagerDatabase::instance().changePassword(_username,newPassword)){
                    QMessageBox::information(this,"Done","Password changing seccessfully!");
                    _userModifierWnd->close();
                }else{
                    QMessageBox::critical(this,"Error","Password changing failed!");
                }
            }
            bool newIsAdministrator=_userModifierWnd->cbxAuthority()->currentIndex();
            if(newIsAdministrator!=_isAdministrator){
                if(ManagerDatabase::instance().changeAuthority(_username,newIsAdministrator)){
                    QMessageBox::information(this,"Done","Authority changing seccessfully!");
                    _isAdministrator=newIsAdministrator;
                    updateUsernameColor();
                    _userModifierWnd->close();
                }else{
                    QMessageBox::critical(this,"Error","Authority changing failed!");
                }
            }
        });

        _userModifierWnd->show();
    });

    connect(ui->btnSelect,&QPushButton::clicked,this,[&](){
        UsersManager::setCurrentUser(_username);
    });

    QMenu* contextMenu = new QMenu(this);
    QAction* deleteAction = new QAction("Delete", this);
    contextMenu->addAction(deleteAction);

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,&UserListItem::customContextMenuRequested,this,[&](const QPoint&)
        {

            QMenu *pMenu = new QMenu;
            pMenu->setStyleSheet("QDialog {\
                                 background: #D6DBE9;\
                             }\
                             QMenu {\
                                 border: 1px solid #CCCCCC;\
                                 border-radius: 3px;\
                                 background-color: #FAFAFC;\
                                 font-size: 22px;\
                                 width:150px;\
                                 text-align:center;\
                                 padding: 5px 0px 5px 0px;\
                             }\
                             QMenu::item {\
                                 text-align:center;\
                             }\
                             QMenu::item:selected {\
                                 background-color: #c84b3b;\
                             }\
                             QMenu::item:disabled{\
                                 color: #CCCCCC;\
                                 background: none;\
                             }\
                             QMenu::separator {\
                                 height: 1px;\
                                 background: #CCCCCC;\
                             }");
            QAction* deleteAction = new QAction("Delete", pMenu);

            connect(deleteAction,&QAction::triggered,this,[&](){
                if(ManagerDatabase::instance().deleteUser(_username)){
                    QMessageBox::information(this,"Done","User deleting successfully!");
                    delete _listWidget->takeItem(_listWidget->currentRow());
                }else{
                    QMessageBox::critical(this,"Error","User deleting failed!");
                }
            });

            pMenu->addAction(deleteAction);
            pMenu->exec(QCursor::pos());

            delete pMenu;
        });
}

void UserListItem::updateUsernameColor(){
    if(_isAdministrator){
        ui->labelUsername->setStyleSheet("font:20px;\n"
                                         "font-weight:500;\n"
                                         "color:rgb(14,150,254);");
    }else{
        ui->labelUsername->setStyleSheet("font:20px;\n"
                                         "font-weight:500;\n"
                                         "color:#888888;");
    }
}

UserListItem::~UserListItem()
{
    delete ui;
}
