#ifndef USERS_MANAGEMENT_WINDOW_H
#define USERS_MANAGEMENT_WINDOW_H

#include <QWidget>
#include "manager_database.h"
#include "user_adder_window.h"

namespace Ui {
class UsersManagementWindow;
}

class UsersManagementWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UsersManagementWindow(QWidget *parent = nullptr);
    void addItem(const QString& username,bool isAdministrator);
    void fillList();
    ~UsersManagementWindow();

private slots:
    void on_btnAdd_clicked();

private:
    Ui::UsersManagementWindow *ui;
    ManagerDatabase* _db;
    UserAdderWindow* _userAdderWnd=nullptr;
};

#endif // USERS_MANAGEMENT_WINDOW_H
