#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <QObject>

class PasswordManager
{
public:
    PasswordManager(const QString& password);

    bool CheckPassword(const QString& password);

    QString EncodePassword(const QString& password);

private:
    QString _password;
};

#endif // PASSWORDMANAGER_H
