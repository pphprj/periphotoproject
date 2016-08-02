#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _cfg = new Configurator();

    _dbm = new DatabaseManager();
    _dbm->Connect(_cfg->GetHost(), _cfg->GetUsername(), _cfg->GetPassword());

    //create combobox with cheking elems for formworks
    _dbm->SelectFormworkSystems(_formworkSystems);
    FillCombobox(_formworkSystems, ui->comboBoxSystems);
    connect(ui->comboBoxSystems->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxSystems_ModelItemChanged(QStandardItem*)));

    //create combobox with cheking elems for features
    _dbm->SelectFeatures(_features);
    FillCombobox(_features, ui->comboBoxFeatures);
    connect(ui->comboBoxFeatures->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxFeatures_ModelItemChanged(QStandardItem*)));

    _dbm->SelectCategories(_categories);

    ui->dateEditProjectDate->setDateTime(QDateTime::currentDateTime());

    _fileManager = new Filemanager(_cfg->GetProjectsDirectory());
}

MainWindow::~MainWindow()
{
    delete _fileManager;
    delete _dbm;
    delete _cfg;
    delete ui;
}

void MainWindow::on_pushButtonTestSQL_clicked()
{
    return;
    DatabaseManager dbm;
    dbm.Connect(_cfg->GetHost(), _cfg->GetUsername(), _cfg->GetPassword());
    dbm.InsertTestValuesToCategoriesTable();
    dbm.InsertTestValuesToFeaturesTable();
    dbm.InsertTestValuesToFormworkSystemsTable();
}

void MainWindow::on_pushButtonLoadPhoto_clicked()
{
    ;
    ui->listWidgetPhotos->clear();

    QFileDialog fileDialog(this,"Open images",_cfg->GetLastFolder(), "Images (*.jpg)" );
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    if (!fileDialog.exec())
    {
        return;
    }

    _files = fileDialog.selectedFiles();
    _cfg->SetLastFolder(fileDialog.directory().absolutePath());

    if (!_files.empty())
    {
        ui->listWidgetPhotos->addItems(_files);
    }
}

void MainWindow::on_pushButtonAddToDB_clicked()
{
    ;
    QString projectNo = GetProjectNo();
    if (projectNo.isEmpty())
    {
        QMessageBox::critical(this, "Error!", "Please, input Project No!");
        return;
    }

    QString selectedFws =  GetSelectedListItems(_formworkSystems, ui->comboBoxSystems->model());
    if (selectedFws.isEmpty())
    {
        QMessageBox::critical(this, "Error!", "Please, select formworks!");
        return;
    }

    QString selectedFeatures = GetSelectedListItems(_features, ui->comboBoxFeatures->model());
    if (selectedFeatures.isEmpty())
    {
        QMessageBox::critical(this, "Error!", "Please, select features!");
        return;
    }

    QString selectedCategories = GetSelectedCategories();
    if (selectedCategories.isEmpty())
    {
        QMessageBox::critical(this, "Error!", "Please, select category!");
        return;
    }

    if (_files.isEmpty())
    {
        QMessageBox::critical(this, "Error!", "Please, select photos!");
        return;
    }

    _fileManager->CreateProjectDirectory(projectNo, GetDateTime());

    QStringList destinationFiles = _fileManager->AddFilesToDirectory(GetFileList());
    bool result = _dbm->InsertValuesToPhotos(projectNo,
                               GetDateTime(),
                               selectedFws,
                               selectedFeatures,
                               selectedCategories,
                               destinationFiles);

    if (result)
    {
        QMessageBox::information(this, "Successfully", "Photos were added to DB");
        ClearInterface();
    }
}

QString MainWindow::GetProjectNo()
{
    return ui->lineEditProjectNo->text();
}

QDateTime MainWindow::GetDateTime()
{
    return ui->dateEditProjectDate->dateTime();
}

template <typename T> QString MainWindow::CreateIDsList(const QVector<T>& elems)
{
    QString result;

    for(int i = 0; i < elems.length(); i++)
    {
        T elem = elems[i];
        result += QString::number(elem.GetID()) + ";";
    }

    result.remove(result.length() - 1, 1);

    return result;
}

template <typename T> void MainWindow::FillCombobox(const QVector<T>& elems, const QComboBox* comboBox)
{
    QStandardItemModel* model = new QStandardItemModel(elems.length() + 1, 1);

    QStandardItem* itemDisableAll = new QStandardItem("Disable all");

    itemDisableAll->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    itemDisableAll->setData(Qt::Unchecked, Qt::CheckStateRole);

    model->setItem(0, 0, itemDisableAll);

    for (int i = 1; i < elems.length() + 1; i++)
    {
        T elem = elems[i - 1];
        QStandardItem* item = new QStandardItem(elem.GetName());

        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);

        model->setItem(i, 0, item);
    }

    QComboBox* cb = const_cast<QComboBox*>(comboBox);
    cb->setModel(model);
    cb->setCurrentIndex(1);
}

template <typename T> QString MainWindow::GetSelectedListItems(const QVector<T>& elems, const QAbstractItemModel *model)
{
    QVector<T> selected;
    QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(model));
    int length = standardModel->rowCount();
    for (int i = 1; i < length; i++)
    {
        if (standardModel->item(i)->checkState() == Qt::Checked)
        {
            selected.push_back(elems[i - 1]);
        }
    }
    return CreateIDsList(selected);
}

QString MainWindow::GetSelectedCategories()
{
    QVector<Categorie> selected;

    if (ui->checkBoxInProc->isChecked()) selected.push_back(_categories[0]);
    if (ui->checkBoxCurrentState->isChecked()) selected.push_back(_categories[1]);
    if (ui->checkBoxMarketing->isChecked()) selected.push_back(_categories[2]);

    return CreateIDsList(selected);
}

QStringList& MainWindow::GetFileList()
{
    return _files;
}

void MainWindow::ClearComboboxChecked(const QAbstractItemModel *model)
{
    QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(model));
    for (int i = 0; i < standardModel->rowCount(); i++)
    {
        if (standardModel->item(i)->checkState() == Qt::Checked)
        {
            standardModel->item(i)->setCheckState(Qt::Unchecked);
        }
    }
}

void MainWindow::ClearInterface()
{
    ui->listWidgetPhotos->clear();
    _files.clear();
    //ClearComboboxChecked(ui->comboBoxFeatures->model());
    //ClearComboboxChecked(ui->comboBoxSystems->model());
}

void MainWindow::on_comboBoxSystems_ModelItemChanged(QStandardItem *item)
{
    if (item->index().row() == 0)
    {
        ClearComboboxChecked(ui->comboBoxSystems->model());
    }
}

void MainWindow::on_comboBoxFeatures_ModelItemChanged(QStandardItem *item)
{
    if (item->index().row() == 0)
    {
        ClearComboboxChecked(ui->comboBoxFeatures->model());
    }
}

