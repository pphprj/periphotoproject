#include "photoloader.h"
#include "tableabstractelemmanager.h"

PhotoLoader::PhotoLoader(DatabaseManager* manager, QObject *parent) : QObject(parent)
{
    _dbm = manager;
}

bool PhotoLoader::LoadDatabase()
{
    bool result = false;
    if(_dbm)
    {
        _formworkSystems.clear();
        result |= _dbm->SelectFormworkSystems(_formworkSystems);
        _features.clear();
        result |= _dbm->SelectFeatures(_features);
        _categories.clear();
        result |= _dbm->SelectCategories(_categories);
        _projectNames.clear();
        result |= _dbm->SelectProjectNames(_projectNames);
    }
    return result;
}

bool PhotoLoader::RefreshProjectNames()
{
    bool result = false;
    if(_dbm)
    {
        _projectNames.clear();
        result |= _dbm->SelectProjectNames(_projectNames);
    }

    return result;
}

bool PhotoLoader::InsertToDatabase(QString& projectNo, QString& projectName, QDate& projectDate, QString& companyName, QString& description,
                                   QVector<FormworkSystem>& selectedFormworks, QVector<Feature>& selectedFeatures, QString& selectedCategories,
                                   QVector<FileInfoStruct>& files, QVector<QFileInfo>& previews)
{
    QString selectedFws = TableAbstractElemManager::CreateIDsList(selectedFormworks);

    QString selectedFts = TableAbstractElemManager::CreateIDsList(selectedFeatures);

    int projectID = _dbm->CheckProjectNo(projectNo);
    if (projectID == -1)
    {
        qDebug() << "projectID == -1";
        return false;
    }

    if (projectID == 0)
    {
        if (_dbm->InsertProject(projectNo, projectName, projectDate, companyName, description))
        {
            projectID = _dbm->CheckProjectNo(projectNo);
        }
    }
    else
    {
        QString projectNameFromDB;
        _dbm->SelectProjectName(projectID, projectNameFromDB);
        if (projectNameFromDB.isEmpty())
        {
            _dbm->UpdateProjectName(projectID, projectName);
        }

        QString companyNameFromDB;
        _dbm->SelectCompanyName(projectID, companyNameFromDB);
        if (companyNameFromDB.isEmpty())
        {
            _dbm->UpdateCompanyName(projectID, companyName);
        }
    }

    bool result = _dbm->InsertValuesToPhotos(projectID,
                                             selectedFws,
                                             selectedFts,
                                             selectedCategories,
                                             files,
                                             previews);

    return result;
}

QVector<FormworkSystem>& PhotoLoader::GetFormworkSystems()
{
    return _formworkSystems;
}

QVector<Feature>& PhotoLoader::GetFeatures()
{
    return _features;
}

QVector<Categorie>& PhotoLoader::GetCategories()
{
    return _categories;
}

QVector<ProjectName>& PhotoLoader::GetProjectNames()
{
    return _projectNames;
}

ProjectName PhotoLoader::GetProjectNameByNo(const QString &projectNo)
{
    foreach (ProjectName name, _projectNames)
    {
        if (name.GetProjectNo() == projectNo)
        {
            return name;
        }
    }
    return ProjectName();
}

ProjectName PhotoLoader::GetProjectNoByName(const QString &projectName)
{
    foreach (ProjectName name, _projectNames)
    {
        if (name.GetName() == projectName)
        {
            return name;
        }
    }
    return ProjectName();
}
