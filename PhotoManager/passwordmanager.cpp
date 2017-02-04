#include "passwordmanager.h"

#include <QCryptographicHash>

PasswordManager::PasswordManager(const QString& password)
{
    _password = password;
}

bool PasswordManager::CheckPassword(const QString &password)
{
    if (password.isEmpty())
    {
        return false;
    }
    return _password == EncodePassword(password);
}

QString PasswordManager::EncodePassword(const QString &password)
{
    QString encode = "";
    encode = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5).toHex();
    encode = encode.toLatin1().toBase64();
    return encode;
}
