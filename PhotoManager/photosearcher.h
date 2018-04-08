#ifndef PHOTOSEARCHER_H
#define PHOTOSEARCHER_H

#include "photoloader.h"

class PhotoSearcher : public PhotoLoader
{
    Q_OBJECT
public:
    explicit PhotoSearcher(DatabaseManager* manager, QObject *parent = 0);

    void SearchPhotos(QString& projectNo, QString& projectName, QDate& projectDate, QString& companyName, QString& address,
                          QVector<FormworkSystem>& selectedFormworks, QVector<Feature>& selectedFeatires, QVector<Categorie>& selectedCategories,
                          QDate& intervalBegin, QDate& intervalEnd,
                          QVector<SearchResult>* photos);

    bool SelectedFormworkSystems(QVector<FormworkSystem>& selectedFormworks, const QString& formworksSystems);
    bool SelectedFormworkSystems(QVector<FormworkSystem>& selectedFormworks, int photoId);
    bool SelectedFeatures(QVector<Feature>& selectedFeatures, const QString& features);
    bool SelectedFeatures(QVector<Feature> &selectedFeatures, int photoId);
    bool SelectedCategories(QVector<Categorie>& selectedCategories, int photoId);

    bool UpdatePhotoAttributes(QVector<FormworkSystem>& selectedFormworks, QVector<Feature>& selectedFeatures, QVector<Categorie>& selectedCategories, int photoId);

public slots:
    void process();

signals:
    void finished();

private:
    QString _projectNo;
    QString _projectName;
    QDate _projectDate;
    QString _companyName;
    QString _address;
    QVector<FormworkSystem> _selectedFormworks;
    QVector<Feature> _selectedFeatures;
    QVector<Categorie> _selectedCategories;
    QDate _intervalBegin;
    QDate _intervalEnd;
    QVector<SearchResult>* _photos;
};

#endif // PHOTOSEARCHER_H
