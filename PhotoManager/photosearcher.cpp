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
