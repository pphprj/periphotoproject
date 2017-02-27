#include "editdbpage.h"

#include "interfacemanager.h"

EditDBPage::EditDBPage(DatabaseManager *databaseManager, Configurator *cfg, QObject *parent)
    : QObject(parent)
{
    _dbm = databaseManager;
    _cfg = cfg;
    _loader = new PhotoLoader(_dbm, parent);

    _dbChangesFlag = false;
}

EditDBPage::~EditDBPage()
{
    delete _loader;
}

void EditDBPage::LoadDatabase()
{
    _loader->LoadDatabase();
}

void EditDBPage::UpdateFormworkSystems()
{
    InterfaceManager::ApplyChanges(_loader->GetFormworkSystems(), _tableFwSystems);
    _dbm->UpdateFormworkSystems(_loader->GetFormworkSystems());
    _dbChangesFlag = false;
}

void EditDBPage::UpdateFeatures()
{
    InterfaceManager::ApplyChanges(_loader->GetFeatures(), _tableFeatures);
    _dbm->UpdateFeatures(_loader->GetFeatures());
    _dbChangesFlag = false;
}

void EditDBPage::InitInterface(QTableWidget *formorkSystems, QTableWidget *features)
{
    _tableFwSystems = formorkSystems;
    _tableFeatures= features;

    connect(_tableFwSystems, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(formworkSystemItemChangedSlot(QTableWidgetItem*)));
    connect(_tableFwSystems, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(featureItemChangedSlot(QTableWidgetItem*)));
}

void EditDBPage::FillInterface()
{
    QStringList tableSystemHeader;
    tableSystemHeader << tr("Formwork system") << tr("Description");
    InterfaceManager::FillTableWidget(tableSystemHeader, _loader->GetFormworkSystems(), _tableFwSystems);

    QStringList tableFeaturesHeader;
    tableFeaturesHeader << tr("Feature") << tr("Description");
    InterfaceManager::FillTableWidget(tableFeaturesHeader, _loader->GetFeatures(), _tableFeatures);

    _dbChangesFlag = false;
}

void EditDBPage::ClearInterface()
{
    _tableFeatures->clear();
    _tableFwSystems->clear();
}

void EditDBPage::formworkSystemItemChangedSlot(QTableWidgetItem *item)
{
    InterfaceManager::ItemChanged(_loader->GetFormworkSystems(), item, _tableFwSystems);
    _dbChangesFlag = true;
}

void EditDBPage::featureItemChangedSlot(QTableWidgetItem *item)
{
    InterfaceManager::ItemChanged(_loader->GetFeatures(), item, _tableFeatures);
    _dbChangesFlag = true;
}


void EditDBPage::NewSystem()
{
    InterfaceManager::NewItem(_tableFwSystems);
    _dbChangesFlag = true;
}

void EditDBPage::NewFeature()
{
    InterfaceManager::NewItem(_tableFeatures);
    _dbChangesFlag = true;
}


bool EditDBPage::DBWasChanged()
{
    return _dbChangesFlag;
}
