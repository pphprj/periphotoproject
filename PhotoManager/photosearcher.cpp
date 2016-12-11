#include "photosearcher.h"
#include "tableabstractelemmanager.h"

PhotoSearcher::PhotoSearcher(DatabaseManager* manager, QObject* parent) :
    PhotoLoader(manager, parent)
{

}

bool PhotoSearcher::SearchPhotos(QString &projectNo,
                                 QString &projectName,
                                 QDate &projectDate,
                                 QVector<FormworkSystem> &selectedFormworks,
                                 QVector<Feature> &selectedFeatures,
                                 QVector<Categorie> &selectedCategories,
                                 QDate& intervalBegin,
                                 QDate& intervalEnd,
                                 QVector<FileAndPreview>& photos)
{
    bool result = _dbm->SelectPhotos(projectNo,
                                             projectName,
                                             projectDate,
                                             selectedFormworks,
                                             selectedFeatures,
                                             selectedCategories,
                                             intervalBegin,
                                             intervalEnd,
                                             photos);

    return result;
}

bool PhotoSearcher::SelectedFormworkSystems(QVector<FormworkSystem> &selectedFormworks, int photoId)
{
    bool result = _dbm->SelectFormworkSystemsByPhotoId(selectedFormworks, photoId);
    return result;
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
