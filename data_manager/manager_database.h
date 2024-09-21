#ifndef LOGIN_DATABASE_H
#define LOGIN_DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QDateTime>
#include <QTime>
#include <functional>

using Oximetry=double;
using Temperature=double;

using EachHumanDataProcessing=std::function<void(const Oximetry&,const Temperature&,const QDateTime&)>;
using EachUserProcessing=std::function<void(const QString&,bool)>;

class ManagerDatabase
{
public:
    /// Enumeration type, representing the status of user registration.
    enum SignupStatus {
        signupFailed,signupSuccessful,signupUsernameExisted
    };

    /// Enumeration type that represents the status of a user's login.
    enum LoginStatus {
        loginSuccessful,loginUserNameNotFound,loginPasswordMismatch,loginOvertime,loginNoAuthority
    };

    /// Enumeration type that represents the status of password change.
    enum ChangePasswordStatus{
        changePasswordFailed,changePasswordSuccessful
    };

    /// Enumeration type that represents the status of the deleted user.
    enum DeleteUserStatus{
        deleteUserFailed,deleteUserSuccessful
    };

    /// A static method used to obtain a singleton instance of the `LoginDatabase` class.
    static ManagerDatabase& instance();

    operator QSqlDatabase&(){
        return _db;
    }

    /// Close the database and delete the instance.
    ~ManagerDatabase(){
        if(_isOpen)
            close();
        if(_instance!=nullptr){
            delete _instance;
            _instance = nullptr;
        }
    }

    /// Find all tables' name.
    QStringList tables(){
        return _db.tables();
    }

    /// Method to open a database connection.
    bool open(){
        _isOpen = _db.open();
        return _isOpen;
    }

    /// The method of closing the database connection.
    void close(){
        _isOpen = false;
        _db.close();
    }

    /// Initialize the table "LoginInfo" of this class.
    void init();

    /// Select all usernames and process each of them.
    bool selectUsernames(const EachUserProcessing& eachUserProcessing);

    /// Find the case sensitive username.
    QString correctUsername(const QString& username);

    /// User registration method. It takes the username and password as parameters
    /// and returns a `SignupStatus` enumeration value representing the registration result.
    SignupStatus signup(const QString& username, const QString& password, bool isAdministrator=true);

    /// User login method. It takes the username and password as parameters
    /// and returns a `LoginStatus` enumeration value representing the login result.
    LoginStatus login(const QString& username, const QString& password,bool ignoreAuthority=false);

    /// Method for changing user passwords. It takes the username, old password, and new password as parameters
    /// and returns a `ChangePasswordStatus` enumeration value representing the result of changing the password.
    ChangePasswordStatus changePassword(const QString& username, const QString& oldPassword, const QString& newPassword);
    ChangePasswordStatus changePassword(const QString& username, const QString& newPassword);

    /// Method for deleting users. It takes the username and password as parameters
    /// and returns a `DeleteUserStatus` enumeration value, representing the result of deleting the user.
    DeleteUserStatus deleteUser(const QString& username, const QString& password);
    DeleteUserStatus deleteUser(const QString& username);

    /// Change the authority of a user.
    bool changeAuthority(const QString& username, bool isAdministrator);

    /// Insert a record of body information of one user.
    bool insertUserData(const QString& username,Oximetry oximetry,Temperature temperature);

    /// Search all records of body information of one user that satisfy requirements.
    bool searchUserData(const QString& username,const EachHumanDataProcessing& eachHumanDataProcessing,
                        const QString& columns,const QString where);
    bool searchUserData(const QString& username,const EachHumanDataProcessing& eachHumanDataProcessing,const QString& columns);
private:
    /// Constructor, which is private, cannot directly instantiate the object.
    /// Single instance instances of a class can only be obtained through the `instance()` method.
    ManagerDatabase();

    bool _createUserDataTable(const QString& username);
    bool _query(const QString& sql,const EachHumanDataProcessing& eachHumanDataProcessing);

    /// This function takes a password string as input and uses the `QCryptographicHash` class
    /// to perform SHA256 hash encryption on it. Finally, use the `toHex ()` function
    /// to convert the hash result to a string represented in hexadecimal, and return the encrypted password.
    ///
    /// Please note that the hash algorithm is irreversible, which means that
    /// the original password cannot be restored from the hash value.
    /// Therefore, when users attempt to log in, they need to encrypt the password they provide
    /// using the same hash algorithm and compare the encrypted password with the hash value stored in the database,
    /// rather than decrypting the hash value to compare with the original password.
    QString _hashPassword(const QString& password);

    QSqlDatabase _db;
    bool _isOpen = false;
    static ManagerDatabase* _instance;
};

#endif // LOGIN_DATABASE_H
