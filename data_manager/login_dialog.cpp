#include "login_dialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(const OnLoginSuccessfully& onLoginSuccessfully,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    _db=&ManagerDatabase::instance();
    ui->setupUi(this);

    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->btnLogin->setShortcut(Qt::Key_Return);

    connect(ui->btnLogin,&QPushButton::clicked,this,[&](){
        ui->btnLogin->setText("Logging..."); // Change the text on the button to prompt that the user is logging in 改变按钮上的文字，提示用户正在登录
        QApplication::processEvents();
        const QString& username=ui->lineEditUsername->text();
        const QString& password=ui->lineEditPassword->text();

        if(username.isEmpty()||password.isEmpty()){
            ui->btnLogin->setText("OK");
            QMessageBox::critical(this,"Error","Please enter the username or password!");
        }

        switch(_db->login(username,password)){
            case ManagerDatabase::loginSuccessful:
                {
                    accept();
                    onLoginSuccessfully(_db->correctUsername(username));
                }
                break;
            case ManagerDatabase::loginPasswordMismatch:
                {
                    QMessageBox::critical(this,"Error","Password Mismatch. Please try again!");
                }
                break;
            case ManagerDatabase::loginUserNameNotFound:
                {
                    QMessageBox::critical(this,"Error","User Not Found. Please try again!");
                }
                break;
            case ManagerDatabase::loginOvertime:
                {
                    QMessageBox::critical(this,"Error","More than 7 days have passed since the last initial login!");
                }
                break;
            case ManagerDatabase::loginNoAuthority:
                {
                    QMessageBox::critical(this,"Error","This user is not an administrator!");
                }
                break;
        }
        ui->btnLogin->setText("OK");
    });
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
