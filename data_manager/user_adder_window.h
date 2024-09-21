#ifndef USER_ADDER_WINDOW_H
#define USER_ADDER_WINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "ui_user_adder_window.h"

namespace Ui {
class UserAdderWindow;
}

class UserAdderWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserAdderWindow(QWidget *parent = nullptr);
    ~UserAdderWindow();

    QLineEdit *lineEditUsername(){
        return ui->lineEditUsername;
    }

    QLineEdit *lineEditPassword(){
        return ui->lineEditPassword;
    }

    QComboBox *cbxAuthority(){
        return ui->cbxAuthority;
    }

    QPushButton *btnAdd(){
        return ui->btnAdd;
    }

private:
    Ui::UserAdderWindow *ui;
};

#endif // USER_ADDER_WINDOW_H
