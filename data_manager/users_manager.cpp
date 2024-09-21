#include "users_manager.h"
#include "main_window.h"

QString UsersManager::_username=QString();

const QString& UsersManager::current(){
    return _username;
}
void UsersManager::setCurrentUser(const QString& username){
    _username=username;
    emit MainWindow::instance().updateCurrentUser();
}
