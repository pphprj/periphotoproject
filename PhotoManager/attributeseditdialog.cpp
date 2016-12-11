#include "attributeseditdialog.h"
#include "ui_attributeseditdialog.h"

#include "interfacemanager.h"
#include "tableabstractelemmanager.h"

AttributesEditDialog::AttributesEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttributesEditDialog)
{
    ui->setupUi(this);
}

AttributesEditDialog::~AttributesEditDialog()
{
    delete ui;
}

void AttributesEditDialog::LoadSelected(PhotoSearcher *seacher, int photoId)
{
    _seacher = seacher;
    _photoId = photoId;

    InterfaceManager::FillCombobox(_seacher->GetFormworkSystems(), ui->comboBoxSystems);
    InterfaceManager::FillCombobox(_seacher->GetFeatures(), ui->comboBoxFeatures);

    _selectedFeatures.clear();
    _selectedSystems.clear();

    _seacher->SelectedFormworkSystems(_selectedSystems, photoId);
    InterfaceManager::SetSelectedItems(_selectedSystems, ui->comboBoxSystems->model());
    InterfaceManager::ShowSelection(_selectedSystems, ui->comboBoxSystems);

    _seacher->SelectedFeatures(_selectedFeatures, photoId);
    InterfaceManager::SetSelectedItems(_selectedFeatures, ui->comboBoxFeatures->model());
    InterfaceManager::ShowSelection(_selectedFeatures, ui->comboBoxFeatures);

    QVector<Categorie> selectedCategories;
    _seacher->SelectedCategories(selectedCategories, photoId);

    if (selectedCategories.length() >= 1)
    {
        if (selectedCategories[0].GetID() == 1) ui->checkBoxInProcess->setCheckState(Qt::Checked);
        if (selectedCategories[0].GetID() == 2) ui->checkBoxCurrentState->setCheckState(Qt::Checked);
        if (selectedCategories[0].GetID() == 3) ui->checkBoxMarketing->setCheckState(Qt::Checked);
    }
    if (selectedCategories.length() >= 2)
    {
        if (selectedCategories[1].GetID() == 2) ui->checkBoxCurrentState->setCheckState(Qt::Checked);
        if (selectedCategories[1].GetID() == 3) ui->checkBoxMarketing->setCheckState(Qt::Checked);
    }
    if (selectedCategories.length() == 3)
    {
        if (selectedCategories[2].GetID() == 3) ui->checkBoxMarketing->setCheckState(Qt::Checked);
    }


    connect(ui->comboBoxSystems->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxSystems_ModelItemChanged(QStandardItem*)));
    connect(ui->comboBoxFeatures->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxFeatures_ModelItemChanged(QStandardItem*)));
}

void AttributesEditDialog::on_comboBoxSystems_ModelItemChanged(QStandardItem *item)
{
    if (item->index().row() == 1)
    {
        InterfaceManager::ClearComboboxChecked(ui->comboBoxSystems);
    }
    else
    {
        if (item->index().row() == 0)
        {
            return;
        }

        _selectedSystems = InterfaceManager::GetSelectedListItems(_seacher->GetFormworkSystems(), ui->comboBoxSystems->model());
        InterfaceManager::ShowSelection(_selectedSystems, ui->comboBoxSystems);
    }
}

void AttributesEditDialog::on_comboBoxFeatures_ModelItemChanged(QStandardItem *item)
{
    if (item->index().row() == 1)
    {
        InterfaceManager::ClearComboboxChecked(ui->comboBoxFeatures);
    }
    else
    {
        if (item->index().row() == 0)
        {
            return;
        }

        _selectedFeatures = InterfaceManager::GetSelectedListItems(_seacher->GetFeatures(), ui->comboBoxFeatures->model());
        InterfaceManager::ShowSelection(_selectedFeatures, ui->comboBoxFeatures);
    }
}

QVector<Categorie> AttributesEditDialog::GetSelectedCategories()
{
    QVector<Categorie> selected;

    if (ui->checkBoxInProcess->isChecked()) selected.push_back(_seacher->GetCategories()[0]);
    if (ui->checkBoxCurrentState->isChecked()) selected.push_back(_seacher->GetCategories()[1]);
    if (ui->checkBoxMarketing->isChecked()) selected.push_back(_seacher->GetCategories()[2]);

    return selected;
}

void AttributesEditDialog::on_buttonBox_accepted()
{
    _seacher->UpdatePhotoAttributes(_selectedSystems, _selectedFeatures, GetSelectedCategories(), _photoId);
}


