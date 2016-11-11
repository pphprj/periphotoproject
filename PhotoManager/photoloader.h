#ifndef PHOTOLOADER_H
#define PHOTOLOADER_H

#include <QObject>

#include "databasemanager.h"

class PhotoLoader : public QObject
{
    Q_OBJECT
public:
    explicit PhotoLoader(DatabaseManager* manager, QObject *parent = 0);

    bool LoadDatabase();

    QVector<FormworkSystem>& GetFormworkSystems();
    QVector<Feature>& GetFeatures();
    QVector<Categorie>& GetCategories();
    QVector<ProjectName>& GetProjectNames();

signals:

public slots:

private:
    QVector<FormworkSystem> _formworkSystems;
    QVector<Feature> _features;
    QVector<Categorie> _categories;
    QVector<ProjectName> _projectNames;

    DatabaseManager* _dbm;
};

#endif // PHOTOLOADER_H
