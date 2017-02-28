#ifndef PHOTOLOADER_H
#define PHOTOLOADER_H

#include <QObject>

#include "databasemanager.h"
#include "fileinfostruct.h"

class PhotoLoader : public QObject
{
    Q_OBJECT
public:
    explicit PhotoLoader(DatabaseManager* manager, QObject *parent = 0);

    bool LoadDatabase();
    bool RefreshProjectNames();
    bool InsertToDatabase(QString& projectNo, QString& projectName, QDate& projectDate, QString& companyName, QString& description,
                          QVector<FormworkSystem>& selectedFormworks, QVector<Feature>& selectedFeatires, QString& selectedCategories,
                          QVector<FileInfoStruct>& files, QVector<QFileInfo>& previews);

    QVector<FormworkSystem>& GetFormworkSystems();
    QVector<Feature>& GetFeatures();
    QVector<Categorie>& GetCategories();
    QVector<ProjectName>& GetProjectNames();
    ProjectName GetProjectNameByNo(const QString& projectNo);
    ProjectName GetProjectNoByName(const QString& projectName);

signals:

public slots:

protected:
    DatabaseManager* _dbm;

private:
    QVector<FormworkSystem> _formworkSystems;
    QVector<Feature> _features;
    QVector<Categorie> _categories;
    QVector<ProjectName> _projectNames;

};

#endif // PHOTOLOADER_H
