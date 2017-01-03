#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql>
#include <QDateTime>

#include "formworksystem.h"
#include "feature.h"
#include "categorie.h"
#include "projectname.h"
#include "fileinfostruct.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();
    bool Connect(const QString& host, const QString& username, const QString& password);

    bool CreateTable(const QString& tableName);

    bool SelectFormworkSystems(QVector<FormworkSystem>& elems);
    bool SelectFeatures(QVector<Feature>& elems);
    bool SelectCategories(QVector<Categorie>& elems);
    bool SelectProjectNames(QVector<ProjectName>& elems);
    bool SelectPhotos(const QString& projectNo,
                      const QString& projectName,
                      const QDate& projectDate,
                      const QVector<FormworkSystem>& formworkSystems,
                      const QVector<Feature>& features,
                      const QVector<Categorie>& categories,
                      const QDate& intervalBegin,
                      const QDate& intervalEnd,
                      QVector<SearchResult>& photos);
    bool SelectPreviews(QVector<SearchResult>& photos);
    bool SelectFormworkSystemsByPhotoId(QVector<FormworkSystem>& elems, int photoId);
    bool SelectFeaturesByPhotoId(QVector<Feature>& elems, int photoId);
    bool SelectCategoriesByPhotoId(QVector<Categorie>& elems, int photoId);

    bool UpdateFormworkSystems(const QVector<FormworkSystem>& elems);
    bool UpdateFeatures(const QVector<Feature>& elems);
    bool UpdatePhotoAttributes(const QString& formworkSystems,
                               const QString& features,
                               const QString& categories,
                               int photoId);

    bool InsertTestValuesToCategoriesTable();
    bool InsertTestValuesToFormworkSystemsTable();
    bool InsertTestValuesToFeaturesTable();

    bool InsertValuesToPhotos(const QString& projectNo,
                              const QString& projectName,
                              const QDate& projectDate,
                              const QString& formworkSystems,
                              const QString& features,
                              const QString& categories,
                              const QVector<FileInfoStruct>& photos,
                              const QVector<QFileInfo>& previews);

    bool InsertPreview(int photoID, const QFileInfo& preview);

    bool InsertProject(const QString& projectNo,
                       const QString& name,
                       const QDate& creationDate,
                       const QString& description);

    //return Project ID
    int CheckProjectNo(const QString& projectNo);
    bool CheckTable(const QString& tableName);

private:

    template <typename T> bool SelectElems(QVector<T>& elems, const QString& tableName, int elemId = -1);
    template <typename T> bool SelectElemsFromPhotos(QVector<T>& elems, const QString& what, const QString& table, int photoId);
    template <typename T> bool InsertOrUpdateElems(const QVector<T>& elems);


private:
    QSqlDatabase _db;
    QString _dateTimeFormat;
};

#endif // DATABASEMANAGER_H
