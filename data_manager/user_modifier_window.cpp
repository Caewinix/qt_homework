#include "user_modifier_window.h"
#include <QAbstractItemView>
#include <QDebug>

UserModifierWindow::UserModifierWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserModifierWindow)
{
    ui->setupUi(this);

    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    ui->cbxAuthority->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->cbxAuthority->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    QStringList items;
    items<<"Normal User"<<"Administrator";
    ui->cbxAuthority->addItems(items);
}

UserModifierWindow::~UserModifierWindow()
{
    delete ui;
}
