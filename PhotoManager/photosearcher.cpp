#include "photosearcher.h"

PhotoSearcher::PhotoSearcher(DatabaseManager* manager, QObject* parent) :
    PhotoLoader(manager, parent)
{

}

bool PhotoSearcher::SearchPhotos(QString &projectNo, QString &projectName, QDate &projectDate, QVector<FormworkSystem> &selectedFormworks, QVector<Feature> &selectedFeatires, QString &selectedCategories, QVector<QFileInfo> &files, QVector<QFileInfo> &previews)
{
    return true;
}
