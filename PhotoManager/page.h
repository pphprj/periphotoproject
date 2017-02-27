#ifndef PAGE_H
#define PAGE_H

#include <QObject>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QStandardItem>

#include "photoloader.h"
#include "databasemanager.h"
#include "filemanager.h"
#include "configurator.h"


class Page : public QObject
{
    Q_OBJECT
public:
    explicit Page(DatabaseManager* databaseManager, FileManager* fileManager, Configurator* cfg, QObject *parent = 0);

    virtual ~Page();

    virtual void InitInterface(QLineEdit* projectNo, QLineEdit* projectName, QDateEdit* projectDate,
                       QComboBox* formorkSystems, QComboBox* features, const QVector<QCheckBox*>& categories);

    virtual void FillInterface();

    virtual void LoadDatabase();

    virtual void CreateLoader() = 0;

    virtual void ClearInterface() = 0;

    QVector<Categorie> GetSelectedCategories();

signals:

public slots:

    void formorksSystemModelItemChangedSlot(QStandardItem *item);
    void featuresModelItemChangedSlot(QStandardItem *item);

    void projectNoLineEditTextEditedSlot(const QString& arg1);
    void projectNameLineEditTextEditedSlot(const QString& arg1);

    void formworkSystemItemActivatedSlot(int index);
    void featuresItemActivatedSlot(int index);

protected:
    QLineEdit* _projectNo;
    QLineEdit* _projectName;
    QDateEdit* _projectDate;
    QComboBox* _formorkSystems;
    QComboBox* _features;
    QVector<QCheckBox*> _categories;

    DatabaseManager* _dbm;
    FileManager* _fileManager;
    Configurator* _cfg;

    QVector<FormworkSystem> _selectedSystems;
    QVector<Feature> _selectedFeatures;

    PhotoLoader* _loader;
};

#endif // PAGE_H
