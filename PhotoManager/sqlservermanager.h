#ifndef SQLSERVERMANAGER_H
#define SQLSERVERMANAGER_H

#include <QSql>
#include <QString>
#include <QVector>

class SQLServerManager
{
public:
    SQLServerManager();
    void LoadServerList();

private:
    QVector<QString> servers_;
};

#endif // SQLSERVERMANAGER_H
