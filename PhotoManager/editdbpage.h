#ifndef EDITDBPAGE_H
#define EDITDBPAGE_H

#include <QObject>
#include <QTableWidget>

#include "databasemanager.h"
#include "configurator.h"
#include "photoloader.h"

class EditDBPage : public QObject
{
    Q_OBJECT
public:
    explicit EditDBPage(DatabaseManager* databaseManager, Configurator* cfg, QObject *parent = 0);
    ~EditDBPage();

    void InitInterface(QTableWidget* formorkSystems, QTableWidget* features);
    void FillInterface();
    void ClearInterface();

    void NewSystem();
    void NewFeature();

    void LoadDatabase();
    void UpdateFormworkSystems();
    void UpdateFeatures();

    bool DBWasChanged();

signals:

public slots:
    void formworkSystemItemChangedSlot(QTableWidgetItem* item);
    void featureItemChangedSlot(QTableWidgetItem* item);

private:
    DatabaseManager* _dbm;
    Configurator* _cfg;

    QTableWidget* _tableFwSystems;
    QTableWidget* _tableFeatures;

    PhotoLoader* _loader;
    bool _dbChangesFlag;
};

#endif // EDITDBPAGE_H
