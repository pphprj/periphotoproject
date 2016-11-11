#include "photoloader.h"

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
