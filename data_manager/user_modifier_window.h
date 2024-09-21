#ifndef USER_MODIFIER_WINDOW_H
#define USER_MODIFIER_WINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "ui_user_modifier_window.h"

namespace Ui {
class UserModifierWindow;
}

class UserModifierWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserModifierWindow(QWidget *parent = nullptr);
    ~UserModifierWindow();

//    QLineEdit *lineEditUsername(){
//        return ui->lineEditUsername;
//    }
    QLabel* usernameDisplay(){
        return ui->usernameDisplay;
    }

    QLineEdit *lineEditPassword(){
        return ui->lineEditPassword;
    }

    QComboBox *cbxAuthority(){
        return ui->cbxAuthority;
    }

    QPushButton *btnConfirm(){
        return ui->btnConfirm;
    }


private:
    Ui::UserModifierWindow *ui;
};

#endif // USER_MODIFIER_WINDOW_H
