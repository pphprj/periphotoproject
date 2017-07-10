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
                         QLineEdit *companyName, QLineEdit *address, QPlainTextEdit *description,
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

    _companyName = companyName;
    _address = address;
    _description = description;

    connect(_projectNo, SIGNAL(textEdited(QString)), this, SLOT(projectNoLineEditTextEditedSlot(QString)));
    connect(_projectNo, SIGNAL(returnPressed()), this, SLOT(projectNoLineEditReturnPressed()));
    connect(_projectName, SIGNAL(textEdited(QString)), this, SLOT(projectNameLineEditTextEditedSlot(QString)));
    connect(_companyName, SIGNAL(textEdited(QString)), this, SLOT(companyNameLineEditTextEditedSlot(QString)));
}

void Page::FillInterface()
{
    //create combobox with cheking elems for formworks
    InterfaceManager::FillCombobox(_loader->GetFormworkSystems(), _formorkSystems);
    connect(_formorkSystems->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(formorksSystemModelItemChangedSlot(QStandardItem*)));

    //create combobox with cheking elems for features
    InterfaceManager::FillCombobox(_loader->GetFeatures(), _features);
    connect(_features->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(featuresModelItemChangedSlot(QStandardItem*)));

    disconnect(_formorkSystems, SIGNAL(activated(int)), 0, 0);
    connect(_formorkSystems, SIGNAL(activated(int)), this, SLOT(formworkSystemItemActivatedSlot(int)));

    disconnect(_features, SIGNAL(activated(int)), 0, 0);
    connect(_features, SIGNAL(activated(int)), this, SLOT(featuresItemActivatedSlot(int)));

    _selectedFeatures.clear();
    _selectedSystems.clear();
}

void Page::LoadDatabase()
{
    _loader->LoadDatabase();
}

QVector<Categorie> Page::GetSelectedCategories()
{
    QVector<Categorie> selected;

    for (int i = 0; i < _categories.length(); i++)
    {
        if (_categories[i]->isChecked())
        {
            selected.push_back(_loader->GetCategories()[i]);
        }
    }

    return selected;
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

void Page::projectNoLineEditReturnPressed()
{
    ProjectName projectName = _loader->GetProjectNameByNo(_projectNo->text());
    _projectName->setText(projectName.GetName());
    _companyName->setText(projectName.GetCompanyName());
    _address->setText(projectName.GetAddress());

    if (_description != nullptr)
    {
        _description->clear();
        _description->appendPlainText(projectName.GetDesription());
    }
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

void Page::companyNameLineEditTextEditedSlot(const QString &arg1)
{
    QStringList items;

    foreach (ProjectName name, _loader->GetProjectNames())
    {
        if (name.GetCompanyName().contains(arg1, Qt::CaseInsensitive))
        {
            if (!items.contains(name.GetCompanyName()))
            {
                items.push_back(name.GetCompanyName());
            }
        }
    }

    InterfaceManager::SetCompleter(_companyName, items);
}

void Page::formworkSystemItemActivatedSlot(int index)
{
    QStandardItem* item = InterfaceManager::ActivateComboBoxItem(_formorkSystems->model(), index);
    formorksSystemModelItemChangedSlot(item);
}

void Page::featuresItemActivatedSlot(int index)
{
    QStandardItem* item = InterfaceManager::ActivateComboBoxItem(_features->model(), index);
    featuresModelItemChangedSlot(item);
}

