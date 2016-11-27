#include "photosearcher.h"
#include "tableabstractelemmanager.h"

PhotoSearcher::PhotoSearcher(DatabaseManager* manager, QObject* parent) :
    PhotoLoader(manager, parent)
{

}

bool PhotoSearcher::SearchPhotos(QString &projectNo, QString &projectName, QDate &projectDate, QVector<FormworkSystem> &selectedFormworks, QVector<Feature> &selectedFeatures, QVector<Categorie> &selectedCategories, QVector<QFileInfo> &files, QVector<QFileInfo> &previews)
{
    bool result = _dbm->SelectPhotos(projectNo,
                                             projectName,
                                             projectDate,
                                             selectedFormworks,
                                             selectedFeatures,
                                             selectedCategories,
                                             files,
                                             previews);

    return result;
}
