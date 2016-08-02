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

    _dbm->SelectFormworkSystems(_formworkSystems);

    //create combobox with cheking elems for formworks
    QStandardItemModel* formworkModel = new QStandardItemModel(_formworkSystems.length(), 1);
    for (int i = 0; i < _formworkSystems.length(); i++)
    {
        QStandardItem* item = new QStandardItem(_formworkSystems[i].GetName());

        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);

        formworkModel->setItem(i, 0, item);
    }
    ui->comboBoxSystems->setModel(formworkModel);

    _dbm->SelectFeatures(_features);

    //create combobox with cheking elems for features
    QStandardItemModel* featuresModel = new QStandardItemModel(_features.length(), 1);
    for (int i = 0; i < _features.length(); i++)
    {
        QStandardItem* item = new QStandardItem(_features[i].GetName());

        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);

        featuresModel->setItem(i, 0, item);
    }
    ui->comboBoxFeatures->setModel(featuresModel);

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

    _files = QFileDialog::getOpenFileNames(this,
                                                      "Open images",
                                                      "/home",
                                                      "Images (*.jpg)" );
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
        ui->listWidgetPhotos->clear();
        _files.clear();
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

template <typename T> QString MainWindow::GetSelectedListItems(const QVector<T>& elems, const QAbstractItemModel *model)
{
    QVector<T> selected;
    QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(model));
    for (int i = 0; i < standardModel->rowCount(); i++)
    {
        if (standardModel->item(i)->checkState() == Qt::Checked)
        {
            selected.push_back(elems[i]);
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
