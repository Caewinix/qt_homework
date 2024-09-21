#ifndef USERSMANAGER_H
#define USERSMANAGER_H
#include <QString>

class UsersManager
{
public:
    static const QString& current();
    static void setCurrentUser(const QString& username);
private:
    UsersManager();
    static QString _username;
};

#endif // USERSMANAGER_H
