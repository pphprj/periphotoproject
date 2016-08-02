#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql>
#include <QDateTime>

#include "formworksystem.h"
#include "feature.h"
#include "categorie.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();
    bool Connect(const QString& host, const QString& username, const QString& password);

    bool SelectFormworkSystems(QVector<FormworkSystem>& elems);
    bool SelectFeatures(QVector<Feature>& elems);
    bool SelectCategories(QVector<Categorie>& elems);

    bool InsertTestValuesToCategoriesTable();
    bool InsertTestValuesToFormworkSystemsTable();
    bool InsertTestValuesToFeaturesTable();

    bool InsertValuesToPhotos(const QString& projectNo,
                                                    const QDateTime& time,
                                                    const QString& formworkSystems,
                                                    const QString& features,
                                                    const QString& categories,
                                                    const QStringList& photos);

    bool InsertProject(const QString& projectNo,
                       const QString& name,
                       const QDateTime& creationTime,
                       const QString& description);

    //return Project ID
    int CheckProjectNo(const QString& projectNo);

private:

    template <typename T> bool SelectElems(QVector<T>& elems, const QString& tableName);

private:
    QSqlDatabase _db;
};

#endif // DATABASEMANAGER_H
