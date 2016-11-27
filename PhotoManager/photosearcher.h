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
                          QVector<QFileInfo>& files, QVector<QFileInfo>& previews);
};

#endif // PHOTOSEARCHER_H
