#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <functional>
#include "ui_login_dialog.h"
#include "manager_database.h"

namespace Ui {
class LoginDialog;
}

typedef std::function<void(const QString&)> OnLoginSuccessfully;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(const OnLoginSuccessfully& onLoginSuccessfully, QWidget *parent = nullptr);
    ~LoginDialog();

    void closeEvent(QCloseEvent *){
        exit(0);
    }

    QLineEdit* lineEditUsername(){
        return ui->lineEditUsername;
    }

    QLineEdit* lineEditPassword(){
        return ui->lineEditPassword;
    }

    QPushButton* btnLogin(){
        return ui->btnLogin;
    }

private:
    Ui::LoginDialog *ui;
    ManagerDatabase* _db;
};

#endif // LOGIN_DIALOG_H
