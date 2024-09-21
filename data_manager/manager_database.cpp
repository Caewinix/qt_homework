#include "manager_database.h"
#include <QStringList>
#include <QCryptographicHash>
#include <QVariant>
#include <QSqlRecord>
#include <QDebug>

QString getCurrentDateAsString()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 milliseconds = currentDateTime.time().msec();

    // 将毫秒四舍五入到最接近的秒
    qint64 roundedMilliseconds = qRound(static_cast<double>(milliseconds));


    // 设置四舍五入后的毫秒值
    QTime roundedTime = currentDateTime.time().addMSecs(roundedMilliseconds - milliseconds);
    currentDateTime.setTime(roundedTime);

    QString formattedDate = currentDateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
    return formattedDate;
}


ManagerDatabase* ManagerDatabase::_instance = nullptr;

// Singleton Pattern Implementation
ManagerDatabase& ManagerDatabase::instance(){
    if(_instance == nullptr){
        _instance = new ManagerDatabase();
    }
    return *_instance;
}

// Connect to the SQLite
ManagerDatabase::ManagerDatabase(){
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        _db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // Establish a connection to the SQLite database
        _db = QSqlDatabase::addDatabase("QSQLITE");
        // Set the name of the database file
        _db.setDatabaseName("manager.db");
    }
    open();
    init();
}

// Create table if not existed.
void ManagerDatabase::init(){
    if(_isOpen){
        if(!_db.tables().contains("LoginInfo")){
            QSqlQuery sqlQuery;
            sqlQuery.exec("CREATE TABLE LoginInfo (\
                          username TEXT PRIMARY KEY NOT NULL,\
                          password TEXT NOT NULL,\
                          loginTime TEXT NOT NULL,\
                          authority INT NOT NULL)");
            signup("admin","123");
        }
    }
}

bool ManagerDatabase::selectUsernames(const EachUserProcessing& eachUserProcessing){
    QSqlQuery query;
    bool success=query.exec(QString("SELECT username,authority FROM LoginInfo"));
    while(query.next()){
        // When the data found in the database has the next line
        eachUserProcessing(query.value(0).toString(),query.value(1).toBool());
        // Call the callback function of each row of data
    }
    return success;
}

QString ManagerDatabase::correctUsername(const QString& username){
    QSqlQuery sqlQuery;
    if(sqlQuery.exec(QString("SELECT username FROM LoginInfo WHERE UPPER(username) = \'%1\'").arg(username.toUpper()))){
        sqlQuery.next();
        return sqlQuery.value(0).toString();
    }else{
        return "";
    }
}

bool ManagerDatabase::_createUserDataTable(const QString& username){
    if(!_db.tables().contains(username)){
        QSqlQuery sqlQuery;
        return sqlQuery.exec(QString("CREATE TABLE %1 (\
                              oximetry REAL NOT NULL,\
                              temperature REAL NOT NULL,\
                              saveTime TEXT PRIMARY KEY NOT NULL)").arg(username));
    }
    return false;
}

/// This function is used to insert oximetry and temperature data into the database,
/// as well as the save time at this time. Returns whether the insertion was successful.
/// Author: Juanhua Zhang
bool ManagerDatabase::insertUserData(const QString& username,Oximetry oximetry,Temperature temperature){
    _db.transaction();
    QSqlQuery query;
    QString sql=QString("insert into %1 (oximetry,temperature,saveTime) values (%2,%3,\'%4\')")
            .arg(username).arg(oximetry).arg(temperature).arg(getCurrentDateAsString());
    bool state=query.exec(sql);
    if(state){
        // If the data is successfully inserted.
        _db.commit();
    }else{
        // If the insertion fails, the rollback operation will be performed.
        _db.rollback();
    }
    return state;
}

bool ManagerDatabase::_query(const QString& sql,const EachHumanDataProcessing& eachHumanDataProcessing){
    QSqlQuery query;
    bool success=query.exec(sql);
    while(query.next()){
        // When the data found in the database has the next line
        eachHumanDataProcessing(query.value(0).toDouble(),query.value(1).toDouble(),
                                QDateTime::fromString(query.value(2).toString(),"yyyy-MM-dd hh:mm:ss.zzz"));
        // Call the callback function of each row of data
    }
    return success;
}


bool ManagerDatabase::searchUserData(
        const QString& username,const EachHumanDataProcessing& eachHumanDataProcessing,const QString& columns,const QString where){
    QString sql=QString("select %1 from %2 where %3").arg(columns,username,where);
    return _query(sql,eachHumanDataProcessing);
}


bool ManagerDatabase::searchUserData(const QString& username,const EachHumanDataProcessing& eachHumanDataProcessing,const QString& columns){
    QString sql=QString("select %1 from %2").arg(columns,username);
    return _query(sql,eachHumanDataProcessing);
}

// Encrypted the password.
QString ManagerDatabase::_hashPassword(const QString& password)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    QByteArray hashedData = hash.result();
    QString hashedPassword = QString(hashedData.toHex());
    return hashedPassword;
}

ManagerDatabase::SignupStatus ManagerDatabase::signup(const QString& username, const QString& password,bool isAdministrator){
    QSqlQuery sqlQuery;
    if(sqlQuery.exec(QString("SELECT username FROM LoginInfo WHERE UPPER(username) = \'%1\'").arg(username.toUpper()))){
        // If the username already exists, it cannot be registered again.
        if(sqlQuery.next())
            return SignupStatus::signupUsernameExisted;
    }

    _createUserDataTable(username);

    // Register a new user, including the password and current time.
    return (SignupStatus)sqlQuery.exec(QString("INSERT INTO LoginInfo VALUES(\'%1\',\'%2\',\'%3\',%4)")
                                       .arg(username,_hashPassword(password),QDateTime::currentDateTime().toString()).arg((int)isAdministrator));
}

ManagerDatabase::LoginStatus ManagerDatabase::login(const QString& username, const QString& password,bool ignoreAuthority){
    QSqlQuery sqlQuery;
    sqlQuery.exec(QString("SELECT password,LoginTime,authority FROM LoginInfo WHERE UPPER(username) = \'%1\'").arg(username.toUpper()));
    if(!sqlQuery.next())
        // If the username cannot be found, it indicates that the user does not exist.
        return ManagerDatabase::LoginStatus::loginUserNameNotFound;
    QString hashPassword = sqlQuery.value(0).toString();
    // Compare whether the encryption values of the password are the same.
    if(_hashPassword(password)==hashPassword){
        QDateTime time = QDateTime::fromString(sqlQuery.value(1).toString());
        qDebug()<<sqlQuery.value(2).toBool()<<','<<ignoreAuthority;
        if(!sqlQuery.value(2).toBool()&&!ignoreAuthority){
            return LoginStatus::loginNoAuthority;
        }
        if(QDateTime::currentDateTime().daysTo(time)<=7){
            // If the username cannot be found, it indicates that the user does not exist.
            return LoginStatus::loginSuccessful;
        }else{
            // If it is greater than 7 days, the next successful login will be a new initial login,
            // so save the time at this time, but it failed to login this time.
            sqlQuery = QSqlQuery();
            sqlQuery.exec(QString("UPDATE LoginInfo SET loginTime = \'%1\' WHERE UPPER(username) = \'%2\'")
                          .arg(getCurrentDateAsString(),username.toUpper()));
            return LoginStatus::loginOvertime;
        }
    }else{
        // Password mismatch, login failed.
        return LoginStatus::loginPasswordMismatch;
    }
}

ManagerDatabase::ChangePasswordStatus
ManagerDatabase::changePassword(const QString& username, const QString& oldPassword, const QString& newPassword){
    QSqlQuery sqlQuery;
    if(sqlQuery.exec(QString("SELECT password FROM LoginInfo WHERE UPPER(username) = \'%1\'").arg(username.toUpper())))
    {
        // Find the password for a specific username
        sqlQuery.next();
        QString hashPassword = sqlQuery.value(0).toString();
        if(_hashPassword(oldPassword)==hashPassword){
            // Only when the old password is entered correctly can the password be modified.
            // Simultaneously update the timestamp.
            return (ChangePasswordStatus)sqlQuery
                    .exec(
                        QString("UPDATE LoginInfo SET password = \'%1\',loginTime = \'%2\' WHERE UPPER(username) = \'%3\'")
                        .arg(_hashPassword(newPassword),QDateTime::currentDateTime().toString(),username.toUpper()));
        }
    }
    return ChangePasswordStatus::changePasswordFailed;
}

ManagerDatabase::ChangePasswordStatus
ManagerDatabase::changePassword(const QString& username, const QString& newPassword){
    QSqlQuery sqlQuery;
    return (ChangePasswordStatus)sqlQuery
            .exec(
                QString("UPDATE LoginInfo SET password = \'%1\',loginTime = \'%2\' WHERE UPPER(username) = \'%3\'")
                .arg(_hashPassword(newPassword),QDateTime::currentDateTime().toString(),username.toUpper()));
}

ManagerDatabase::DeleteUserStatus ManagerDatabase::deleteUser(const QString& username, const QString& password){
    QSqlQuery sqlQuery;
    // To delete an account, it is necessary to ensure that both the username and password are correct,
    // and ensure that the operation is done for the specific individual.
    return (DeleteUserStatus)sqlQuery
            .exec(QString("DELETE FROM LoginInfo WHERE UPPER(username) = \'%1\' AND password = \'%2\'")
                  .arg(username.toUpper(),_hashPassword(password)));
}


ManagerDatabase::DeleteUserStatus ManagerDatabase::deleteUser(const QString& username){
    QSqlQuery sqlQuery;
    // To delete an account, it is necessary to ensure that both the username and password are correct,
    // and ensure that the operation is done for the specific individual.
    return (DeleteUserStatus)sqlQuery
            .exec(QString("DELETE FROM LoginInfo WHERE UPPER(username) = \'%1\'")
                  .arg(username.toUpper()));
}

bool ManagerDatabase::changeAuthority(const QString& username, bool isAdministrator){
    QSqlQuery sqlQuery;
    return sqlQuery
            .exec(
                QString("UPDATE LoginInfo SET authority = %1 WHERE UPPER(username) = \'%2\'")
                .arg((int)isAdministrator)
                .arg(username.toUpper()));;
}
