#include "photosearcher.h"
#include "tableabstractelemmanager.h"

PhotoSearcher::PhotoSearcher(DatabaseManager* manager, QObject* parent) :
    PhotoLoader(manager, parent)
{

}

void PhotoSearcher::SearchPhotos(QString &projectNo,
                                 QString &projectName,
                                 QDate &projectDate,
                                 QString& companyName,
                                 QString& address,
                                 QVector<FormworkSystem> &selectedFormworks,
                                 QVector<Feature> &selectedFeatures,
                                 QVector<Categorie> &selectedCategories,
                                 QDate& intervalBegin,
                                 QDate& intervalEnd,
                                 QVector<SearchResult>* photos)
{
    _projectNo = projectNo;
    _projectName = projectName;
    _projectDate = projectDate;
    _companyName = companyName;
    _address = address;
    _selectedFormworks = selectedFormworks;
    _selectedFeatures = selectedFeatures;
    _selectedCategories = selectedCategories;
    _intervalBegin = intervalBegin;
    _intervalEnd = intervalEnd;
    _photos = photos;

   /* bool result = _dbm->SelectPhotos(projectNo,
                                     projectName,
                                     projectDate,
                                     companyName,
                                     address,
                                             selectedFormworks,
                                             selectedFeatures,
                                             selectedCategories,
                                             intervalBegin,
                                             intervalEnd,
                                             photos);*/

}

bool PhotoSearcher::SelectedFormworkSystems(QVector<FormworkSystem> &selectedFormworks, const QString& formworksSystems)
{
    QString elemId = "";
    for (int i = 0; i < formworksSystems.length(); i++)
    {
        if (formworksSystems[i] != ';')
        {
            elemId += formworksSystems[i];
        }
        else
        {
            int photoId = elemId.toInt();
            for (int i = 0; i < _formworkSystems.length(); i++)
            {
                if (_formworkSystems[i].GetID() == photoId)
                {
                    selectedFormworks.push_back(_formworkSystems[i]);
                }
            }
            elemId = "";
        }
    }

    //for last or only one
    int photoId = elemId.toInt();
    for (int i = 0; i < _formworkSystems.length(); i++)
    {
        if (_formworkSystems[i].GetID() == photoId)
        {
            selectedFormworks.push_back(_formworkSystems[i]);
        }
    }

    return true;
}

bool PhotoSearcher::SelectedFormworkSystems(QVector<FormworkSystem> &selectedFormworks, int photoId)
{
    return _dbm->SelectFormworkSystemsByPhotoId(selectedFormworks, photoId);
}

bool PhotoSearcher::SelectedFeatures(QVector<Feature> &selectedFeatures, const QString& features)
{
    QString elemId = "";
    for (int i = 0; i < features.length(); i++)
    {
        if (features[i] != ';')
        {
            elemId += features[i];
        }
        else
        {
            int photoId = elemId.toInt();
            for (int i = 0; i < _features.length(); i++)
            {
                if (_features[i].GetID() == photoId)
                {
                    selectedFeatures.push_back(_features[i]);
                }
            }
            elemId = "";
        }
    }

    //for last or only one
    int photoId = elemId.toInt();
    for (int i = 0; i < _features.length(); i++)
    {
        if (_features[i].GetID() == photoId)
        {
            selectedFeatures.push_back(_features[i]);
        }
    }

    return true;
}

bool PhotoSearcher::SelectedFeatures(QVector<Feature> &selectedFeatures, int photoId)
{
    return _dbm->SelectFeaturesByPhotoId(selectedFeatures, photoId);
}

bool PhotoSearcher::SelectedCategories(QVector<Categorie> &selectedCategories, int photoId)
{
    return _dbm->SelectCategoriesByPhotoId(selectedCategories, photoId);
}

bool PhotoSearcher::UpdatePhotoAttributes(QVector<FormworkSystem> &selectedFormworks, QVector<Feature> &selectedFeatures, QVector<Categorie> &selectedCategories, int photoId)
{
    QString selectedFws = TableAbstractElemManager::CreateIDsList(selectedFormworks);

    QString selectedFts = TableAbstractElemManager::CreateIDsList(selectedFeatures);

    QString selectedCtgs = TableAbstractElemManager::CreateIDsList(selectedCategories);

    return _dbm->UpdatePhotoAttributes(selectedFws, selectedFts, selectedCtgs, photoId);
}

void PhotoSearcher::process()
{
    bool result = _dbm->SelectPhotos(_projectNo,
                                     _projectName,
                                     _projectDate,
                                     _companyName,
                                     _address,
                                     _selectedFormworks,
                                     _selectedFeatures,
                                     _selectedCategories,
                                     _intervalBegin,
                                     _intervalEnd,
                                     _photos);
    emit finished();
}
