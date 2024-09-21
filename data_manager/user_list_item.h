#ifndef USER_LIST_ITEM_H
#define USER_LIST_ITEM_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <functional>
#include "ui_user_list_item.h"
#include "user_modifier_window.h"
#include "users_management_window.h"

typedef std::function<void()> VoidCallback;

namespace Ui {
class UserListItem;
}

class UserListItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserListItem(const QString& username,bool isAdministrator,QListWidget* listWidget,QWidget *parent = nullptr);
    const QString& username(){
        return _username;
    }
    const bool& isAdministrator(){
        return _isAdministrator;
    }
    void updateUsernameColor();
    QPushButton* btnModify(){
        return ui->btnModify;
    }
    QPushButton* btnSelect(){
        return ui->btnSelect;
    }
    ~UserListItem();

private:
    Ui::UserListItem *ui;
    QString _username;
    bool _isAdministrator;
    QListWidget* _listWidget=nullptr;
    UserModifierWindow* _userModifierWnd=nullptr;
};

//class Ui_UserListItem
//{
//public:
//    QHBoxLayout *anotherHorizontalLayout;
//    QSpacerItem *horizontalSpacerLeft;
//    QLabel *labelUsername;
//    QHBoxLayout *horizontalLayout;
//    QPushButton *btnModify;
//    QPushButton *btnSelect;
//    QSpacerItem *horizontalSpacerRight;

//    void setupUi(QWidget *userListItem)
//    {
//        if (userListItem->objectName().isEmpty())
//            userListItem->setObjectName(QString::fromUtf8("UserListItem"));
//        userListItem->resize(619, 100);
//        userListItem->setStyleSheet(QString::fromUtf8("background:rgba(0,0,0,0);"));
//        anotherHorizontalLayout = new QHBoxLayout(userListItem);
//        anotherHorizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout_2"));
//        horizontalSpacerLeft = new QSpacerItem(5, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

//        anotherHorizontalLayout->addItem(horizontalSpacerLeft);

//        labelUsername = new QLabel(userListItem);
//        labelUsername->setObjectName(QString::fromUtf8("labelUsername"));
////        labelUsername->setStyleSheet(QString::fromUtf8("font:20px;\n"
////"font-weight:500;\n"
////"color:#888888;"));
//        labelUsername->setText(((UserListItem*)userListItem)->username());

//        anotherHorizontalLayout->addWidget(labelUsername);

//        horizontalLayout = new QHBoxLayout();
//        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
//        btnModify = new QPushButton(userListItem);
//        btnModify->setObjectName(QString::fromUtf8("btnDelectAndExport"));
//        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
//        sizePolicy.setHorizontalStretch(0);
//        sizePolicy.setVerticalStretch(0);
//        sizePolicy.setHeightForWidth(btnModify->sizePolicy().hasHeightForWidth());
//        btnModify->setSizePolicy(sizePolicy);
//        btnModify->setStyleSheet(QString::fromUtf8("QPushButton\n"
//"{\n"
//"    font:12.5px;\n"
//"	font-weight:600;\n"
//"    color:rgb(14 , 150 , 254);\n"
//"    background-color:white;\n"
//"	border:2.5px solid rgb(14 , 150 , 254);\n"
//"    border-radius:20px;\n"
//"	margin-top:10;\n"
//"	margin-bottom:10\n"
//"}\n"
//"\n"
//"QPushButton:hover\n"
//"{\n"
//"    background-color:rgba(0 ,0 , 0,50);\n"
//"}\n"
//"\n"
//"QPushButton:pressed\n"
//"{\n"
//"    background-color:rgb(14 , 150 , 254);\n"
//"	color:white;\n"
//"}"));

//        horizontalLayout->addWidget(btnModify);

//        btnSelect = new QPushButton(userListItem);
//        btnSelect->setObjectName(QString::fromUtf8("btnSelect"));
//        sizePolicy.setHeightForWidth(btnSelect->sizePolicy().hasHeightForWidth());
//        btnSelect->setSizePolicy(sizePolicy);
//        btnSelect->setStyleSheet(QString::fromUtf8("QPushButton\n"
//"{\n"
//"    font-family:Microsoft Yahei;\n"
//"    font:12.5px;\n"
//"	font-weight:600;\n"
//"    color:rgb(54 , 215 , 82);\n"
//"    background-color:white;\n"
//"	border:2.5px solid rgb(54 , 215 , 82);\n"
//"    border-radius:20px;\n"
//"	margin-top:10;\n"
//"	margin-bottom:10\n"
//"}\n"
//"\n"
//"QPushButton:hover\n"
//"{\n"
//"    background-color:rgba(0 ,0 , 0,50);\n"
//"}\n"
//"\n"
//"QPushButton:pressed\n"
//"{\n"
//"    background-color:rgb(54 , 215 , 82);\n"
//"	color:white;\n"
//"}"));

//        horizontalLayout->addWidget(btnSelect);


//        anotherHorizontalLayout->addLayout(horizontalLayout);

//        horizontalSpacerRight = new QSpacerItem(5, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

//        anotherHorizontalLayout->addItem(horizontalSpacerRight);


//        retranslateUi(userListItem);

//        QMetaObject::connectSlotsByName(userListItem);
//    } // setupUi

//    void retranslateUi(QWidget *userListItem)
//    {
//        userListItem->setWindowTitle(QString());
//        btnModify->setText(QApplication::translate("UserListItem", "MODIFY", nullptr));
//        btnSelect->setText(QApplication::translate("UserListItem", "SELECT", nullptr));
//    } // retranslateUi

//};

//namespace Ui {
//    class UserListItem: public Ui_UserListItem {};
//} // namespace Ui

#endif // USER_LIST_ITEM_H
