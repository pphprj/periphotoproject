#ifndef PHOTOSEARCHER_H
#define PHOTOSEARCHER_H

#include "photoloader.h"

class PhotoSearcher : public PhotoLoader
{
    Q_OBJECT
public:
    explicit PhotoSearcher(DatabaseManager* manager, QObject *parent = 0);

    bool SearchPhotos(QString& projectNo, QString& projectName, QDate& projectDate,
                          QVector<FormworkSystem>& selectedFormworks, QVector<Feature>& selectedFeatires, QVector<Categorie>& selectedCategories,
                          QDate& intervalBegin, QDate& intervalEnd,
                          QVector<SearchResult>& photos);

    bool SelectedFormworkSystems(QVector<FormworkSystem>& selectedFormworks, int photoId);
    bool SelectedFeatures(QVector<Feature>& selectedFeatures, int photoId);
    bool SelectedCategories(QVector<Categorie>& selectedCategories, int photoId);

    bool UpdatePhotoAttributes(QVector<FormworkSystem>& selectedFormworks, QVector<Feature>& selectedFeatures, QVector<Categorie>& selectedCategories, int photoId);
};

#endif // PHOTOSEARCHER_H
