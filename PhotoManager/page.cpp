#include "page.h"

#include "interfacemanager.h"

Page::Page(DatabaseManager *databaseManager,
                           FileManager *fileManager,
                           Configurator *cfg,
                           QObject *parent) : QObject(parent)
{
    _dbm = databaseManager;
    _fileManager = fileManager;
    _cfg = cfg;
}

Page::~Page()
{
}

void Page::InitInterface(QLineEdit *projectNo, QLineEdit *projectName, QDateEdit *projectDate,
                                 QComboBox *formorkSystems, QComboBox *features, const QVector<QCheckBox*>& categories)
{
    _projectNo = projectNo;
    _projectName = projectName;
    _projectDate = projectDate;
    _formorkSystems = formorkSystems;
    _features = features;
    foreach (QCheckBox* cb, categories)
    {
        _categories.push_back(cb);
    }

    connect(_projectNo, SIGNAL(textEdited(QString)), this, SLOT(projectNoLineEditTextEditedSlot(QString)));
    connect(_projectName, SIGNAL(textEdited(QString)), this, SLOT(projectNameLineEditTextEditedSlot(QString)));
}

void Page::FillInterface()
{
    //create combobox with cheking elems for formworks
    InterfaceManager::FillCombobox(_loader->GetFormworkSystems(), _formorkSystems);
    connect(_formorkSystems->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(formorksSystemModelItemChangedSlot(QStandardItem*)));

    //create combobox with cheking elems for features
    InterfaceManager::FillCombobox(_loader->GetFeatures(), _features);
    connect(_features->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(featuresModelItemChangedSlot(QStandardItem*)));

    _selectedFeatures.clear();
    _selectedSystems.clear();
}

void Page::LoadDatabase()
{
    _loader->LoadDatabase();
}

QString Page::GetSelectedCategories()
{
    QVector<Categorie> selected;

    //if (ui->checkBoxInProc->isChecked()) selected.push_back(_loader->GetCategories()[0]);
    //if (ui->checkBoxCurrentState->isChecked()) selected.push_back(_loader->GetCategories()[1]);
    //if (ui->checkBoxMarketing->isChecked()) selected.push_back(_loader->GetCategories()[2]);

    for (int i = 0; i < _categories.length(); i++)
    {
        if (_categories[i]->isChecked())
        {
            selected.push_back(_loader->GetCategories()[i]);
        }
    }

    return TableAbstractElemManager::CreateIDsList(selected);
}

void Page::UpdateFormorkSystems(QTableWidget *table)
{
    InterfaceManager::ApplyChanges(_loader->GetFormworkSystems(), table);
}

void Page::UpdateFeatures(QTableWidget *table)
{
    InterfaceManager::ApplyChanges(_loader->GetFeatures(), table);
}

void Page::formorksSystemModelItemChangedSlot(QStandardItem *item)
{
    if (item->index().row() == 1)
    {
        InterfaceManager::ClearComboboxChecked(_formorkSystems);
    }
    else
    {
        if (item->index().row() == 0)
        {
            return;
        }

        _selectedSystems = InterfaceManager::GetSelectedListItems(_loader->GetFormworkSystems(), _formorkSystems->model());
        InterfaceManager::ShowSelection(_selectedSystems, _formorkSystems);
    }
}

void Page::featuresModelItemChangedSlot(QStandardItem *item)
{
    if (item->index().row() == 1)
    {
        InterfaceManager::ClearComboboxChecked(_features);
    }
    else
    {
        if (item->index().row() == 0)
        {
            return;
        }

        _selectedFeatures = InterfaceManager::GetSelectedListItems(_loader->GetFeatures(), _features->model());
        InterfaceManager::ShowSelection(_selectedFeatures, _features);
    }
}


void Page::projectNoLineEditTextEditedSlot(const QString &arg1)
{
    QStringList items;
    foreach (ProjectName name, _loader->GetProjectNames())
    {
        if (name.GetProjectNo().contains(arg1, Qt::CaseInsensitive))
        {
            items.push_back(name.GetProjectNo());
        }
    }

    InterfaceManager::SetCompleter(_projectNo, items);
}

void Page::projectNameLineEditTextEditedSlot(const QString &arg1)
{
    QStringList items;

    foreach (ProjectName name, _loader->GetProjectNames())
    {
        if (name.GetName().contains(arg1, Qt::CaseInsensitive))
        {
             items.push_back(name.GetName());
        }
    }

    InterfaceManager::SetCompleter(_projectName, items);
}

