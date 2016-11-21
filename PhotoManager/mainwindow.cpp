#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "previewworker.h"
#include "interfacemanager.h"
#include "tableabstractelemmanager.h"

#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    InitInterface();

    InitDatabase();

    LoadDatabase();

    LoadInterface();

    _fileManager = new FileManager(_cfg->GetProjectsDirectory());
    _copierThread = new FilecopierThread(_fileManager);
    _previewSession = new PreviewSession(ui->listWidgetPhotos);

    QObject::connect(_copierThread, SIGNAL(progress(int)), this, SLOT(setProgressBarValue(int)));
    QObject::connect(_copierThread, SIGNAL(finished()), this, SLOT(finishedCopy()));

    _dbChangesFlag = false;
}

MainWindow::~MainWindow()
{
    delete _loader;
    delete _previewSession;
    delete _copierThread;
    delete _fileManager;
    delete _dbm;
    delete _cfg;
    delete ui;
}

void MainWindow::InitDatabase()
{
    _cfg = new Configurator();

    _dbm = new DatabaseManager();
    bool result = _dbm->Connect(_cfg->GetHost(), _cfg->GetUsername(), _cfg->GetPassword());
    if (result)
    {
        _loader = new PhotoLoader(_dbm);
    }

    qDebug() << "db connect result " << result;
}

void MainWindow::InitInterface()
{
    ui->setupUi(this);
    ui->statusBar->setVisible(false);
    ui->menuBar->setVisible(false);

    setAcceptDrops(true);

    //set captions for i18n
    ui->groupBoxCategories->setTitle(tr("Categories"));
    ui->groupBoxFeatures->setTitle(tr("Features"));
    ui->groupBoxPhotos->setTitle(tr("Photos"));
    ui->groupBoxProject->setTitle(tr("Project"));
    ui->groupBoxSystems->setTitle(tr("Formwork systems"));
    ui->labelFilesCopied->setText(tr("Files were copied"));
    ui->labelProjectDate->setText(tr("Project creation date"));
    ui->labelProjectFeatures->setText(tr("Features"));
    ui->labelProjectName->setText(tr("Project name"));
    ui->labelProjectNo->setText(tr("Project No"));
    ui->labelSystems->setText(tr("Formwork systems"));
    ui->checkBoxCurrentState->setText(tr("Current state"));
    ui->checkBoxEnablePreview->setText(tr("Enable preview"));
    ui->checkBoxInProc->setText(tr("In process"));
    ui->checkBoxMarketing->setText(tr("Marketing"));
    ui->pushButtonAddToDB->setText(tr("Add to DB"));
    ui->pushButtonApplyFeature->setText(tr("Apply"));
    ui->pushButtonApplySystem->setText(tr("Apply"));
    ui->pushButtonLoadPhoto->setText(tr("Load photo"));
    ui->pushButtonNewFeature->setText(tr("New feature"));
    ui->pushButtonSystemsNew->setText(tr("New system"));
    ui->tabWidgetSystem->setTabText(0, tr("Add new photos"));
    ui->tabWidgetSystem->setTabText(1, tr("Edit database"));

    ui->dateEditProjectDate->setDateTime(QDateTime::currentDateTime());
    ui->listWidgetPhotos->setIconSize(QSize(0, 0));
    ui->listWidgetPhotos->installEventFilter(this);
}

void MainWindow::LoadDatabase()
{
    if (_loader)
    {
        _loader->LoadDatabase();
    }
}

void MainWindow::LoadInterface()
{
    //create combobox with cheking elems for formworks
    InterfaceManager::FillCombobox(_loader->GetFormworkSystems(), ui->comboBoxSystems);
    connect(ui->comboBoxSystems->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxSystems_ModelItemChanged(QStandardItem*)));

    //create combobox with cheking elems for features
    InterfaceManager::FillCombobox(_loader->GetFeatures(), ui->comboBoxFeatures);
    connect(ui->comboBoxFeatures->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxFeatures_ModelItemChanged(QStandardItem*)));

    ui->checkBoxEnablePreview->setChecked(true);
    on_checkBoxEnablePreview_clicked();
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

void MainWindow::AddFileToPreview(const QString &fileName)
{
    if (!_files.contains(fileName, Qt::CaseInsensitive))
    {
        _files.push_back(fileName);

        _previewSession->addFile(fileName);
    }
}

void MainWindow::DeleteFileFromPreview(int num)
{
    if ((num < 0) || (num > _files.length()))
    {
        return;
    }
    delete ui->listWidgetPhotos->item(num);
    _files.removeAt(num);
}

void MainWindow::on_pushButtonLoadPhoto_clicked()
{
    QFileDialog fileDialog(this, tr("Open images"), _cfg->GetLastFolder(), tr("Images (*.jpg)"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    if (!fileDialog.exec())
    {
        return;
    }

    QStringList selectedFiles = fileDialog.selectedFiles();
    _cfg->SetLastFolder(fileDialog.directory().absolutePath());

    if (!selectedFiles.empty())
    {
        foreach (QString str, selectedFiles)
        {
            AddFileToPreview(str);
        }
    }
}

void MainWindow::on_pushButtonAddToDB_clicked()
{
    QString projectNo = GetProjectNo();
    if (projectNo.isEmpty())
    {
        QMessageBox::critical(this, tr("Error!"), tr("Please, input Project No!"));
        return;
    }

    if (_selectedSystems.isEmpty())
    {
        QMessageBox::critical(this, tr("Error!"), tr("Please, select formworks!"));
        return;
    }

    if (_selectedFeatures.isEmpty())
    {
        QMessageBox::critical(this, tr("Error!"), tr("Please, select features!"));
        return;
    }

    QString selectedCategories = GetSelectedCategories();
    if (selectedCategories.isEmpty())
    {
        QMessageBox::critical(this, tr("Error!"), tr("Please, select category!"));
        return;
    }

    if (_files.isEmpty())
    {
        QMessageBox::critical(this, tr("Error!"), tr("Please, select photos!"));
        return;
    }

    QString projectName = GetProjectName();
    _fileManager->CreateProjectDirectory(projectNo, projectName);
    _copierThread->setFileList(_files, _cfg->GetCompressionRate());
    _copierThread->start();
}

QString MainWindow::GetProjectNo()
{
    return ui->lineEditProjectNo->text();
}

QString MainWindow::GetProjectName()
{
    return ui->lineEditProjectName->text();
}

QDate MainWindow::GetProjectDate()
{
    return ui->dateEditProjectDate->date();
}

void MainWindow::GetFilesDate()
{
}


QString MainWindow::GetSelectedCategories()
{
    QVector<Categorie> selected;

    if (ui->checkBoxInProc->isChecked()) selected.push_back(_loader->GetCategories()[0]);
    if (ui->checkBoxCurrentState->isChecked()) selected.push_back(_loader->GetCategories()[1]);
    if (ui->checkBoxMarketing->isChecked()) selected.push_back(_loader->GetCategories()[2]);

    return TableAbstractElemManager::CreateIDsList(selected);
}

void MainWindow::ClearInterface()
{
    ui->listWidgetPhotos->clear();
    ui->progressBar->setValue(0);
    _files.clear();
}

void MainWindow::on_comboBoxSystems_ModelItemChanged(QStandardItem *item)
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

        _selectedSystems = InterfaceManager::GetSelectedListItems(_loader->GetFormworkSystems(), ui->comboBoxSystems->model());
        InterfaceManager::ShowSelection(_selectedSystems, ui->comboBoxSystems);
    }
}

void MainWindow::on_comboBoxFeatures_ModelItemChanged(QStandardItem *item)
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

        _selectedFeatures = InterfaceManager::GetSelectedListItems(_loader->GetFeatures(), ui->comboBoxFeatures->model());
        InterfaceManager::ShowSelection(_selectedFeatures, ui->comboBoxFeatures);
    }
}


void MainWindow::EnableDranNDrop()
{
    ui->listWidgetPhotos->setDragDropMode(QAbstractItemView::InternalMove);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QRect r = ui->groupBoxPhotos->geometry();
    QPoint pos = event->pos();
    if (r.contains(pos))
    {
        if ((event->mimeData()->text().contains(".jpg", Qt::CaseInsensitive)) ||
            (event->mimeData()->text().contains(".jpeg", Qt::CaseInsensitive)))
        {
            if (event->mimeData()->hasUrls())
            {
                QList<QUrl> urlList = event->mimeData()->urls();

                // extract the local paths of the files
                for (int i = 0; i < urlList.size(); i++)
                {
                    QString fileName = urlList.at(i).toLocalFile();
                    AddFileToPreview(fileName);
                }
            }
        }
    }
    event->acceptProposedAction();
}

void MainWindow::on_tabWidgetSystem_currentChanged(int index)
{
    if (index == 1)
    {
        InterfaceManager::FillTableWidget(_loader->GetFormworkSystems(), ui->tableWidgetSystems);
        InterfaceManager::FillTableWidget(_loader->GetFeatures(), ui->tableWidgetFeatures);

        _dbChangesFlag = false;
    }

    if (index == 0)
    {
        ui->tableWidgetSystems->clear();
        ui->tableWidgetFeatures->clear();
    }
}


void MainWindow::on_pushButtonApplySystem_clicked()
{
    if (!ConfirmWindow())
        return;

    InterfaceManager::ApplyChanges(_loader->GetFormworkSystems(), ui->tableWidgetSystems);
    _dbChangesFlag = false;

    _dbm->UpdateFormworkSystems(_loader->GetFormworkSystems());

    LoadDatabase();
    LoadInterface();
}

void MainWindow::on_tableWidgetSystems_itemChanged(QTableWidgetItem *item)
{
    InterfaceManager::ItemChanged(_loader->GetFormworkSystems(), item, ui->tableWidgetSystems);
    _dbChangesFlag = true;
}

void MainWindow::on_pushButtonSystemsNew_clicked()
{
    InterfaceManager::NewItem(ui->tableWidgetSystems);
    _dbChangesFlag = true;
}

void MainWindow::on_pushButtonNewFeature_clicked()
{
    InterfaceManager::NewItem(ui->tableWidgetFeatures);
    _dbChangesFlag = true;
}

void MainWindow::on_pushButtonApplyFeature_clicked()
{
    if (!ConfirmWindow())
        return;

    InterfaceManager::ApplyChanges(_loader->GetFeatures(), ui->tableWidgetFeatures);
    _dbChangesFlag = false;
    _dbm->UpdateFeatures(_loader->GetFeatures());
    LoadDatabase();
    LoadInterface();
}

void MainWindow::on_tableWidgetFeatures_itemChanged(QTableWidgetItem *item)
{
    InterfaceManager::ItemChanged(_loader->GetFeatures(), item, ui->tableWidgetFeatures);
    _dbChangesFlag = true;
}

void MainWindow::on_tabWidgetSystem_tabBarClicked(int index)
{
    if (index == 0)
    {
        if (ui->tabWidgetSystem->currentIndex() == index)
            return;

        if (!_dbChangesFlag)
            return;

        if (!ConfirmWindow())
            return;

        InterfaceManager::ApplyChanges(_loader->GetFormworkSystems(), ui->tableWidgetSystems);
        _dbChangesFlag = false;
        _dbm->UpdateFormworkSystems(_loader->GetFormworkSystems());

        InterfaceManager::ApplyChanges(_loader->GetFeatures(), ui->tableWidgetFeatures);
        _dbChangesFlag = false;
        _dbm->UpdateFeatures(_loader->GetFeatures());

        LoadDatabase();
        LoadInterface();
    }
}

bool MainWindow::ConfirmWindow()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Apply changes"), tr("Would you have to apply changes?"),
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        return true;

    return false;
}


void MainWindow::on_checkBoxEnablePreview_clicked()
{
    if (ui->checkBoxEnablePreview->isChecked())
    {
        ui->listWidgetPhotos->setViewMode(QListWidget::IconMode);
        ui->listWidgetPhotos->setIconSize(QSize(100, 100));
        ui->listWidgetPhotos->setResizeMode(QListWidget::Adjust);
    }
    else
    {
        ui->listWidgetPhotos->setViewMode(QListWidget::ListMode);
        ui->listWidgetPhotos->setIconSize(QSize(0, 0));
    }
}

void MainWindow::setProgressBarValue(int value)
{
    ui->progressBar->setValue(value);
}

void MainWindow::finishedCopy()
{
    qDebug() << "finishedCopy start";
    QVector<QFileInfo> files = _copierThread->getCopiedFiles();


    QVector<QFileInfo> previews;
    for (int i = 0; i < _files.count(); i++)
    {
       previews.push_back(_fileManager->AddPreviewToDirectory(ui->listWidgetPhotos->item(i)->icon(), _files.at(i)));
    }


    bool result = _loader->InsertToDatabase(GetProjectNo(), GetProjectName(), GetProjectDate(),
                                            _selectedSystems, _selectedFeatures, GetSelectedCategories(),
                                            files,
                                            previews);

    if (result)
    {
        QMessageBox::information(this, tr("Successfully"), tr("Photos were added to DB"));
        ClearInterface();
    }
    qDebug() << "finishedCopy end" << result;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->listWidgetPhotos && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete)
        {
            DeleteFileFromPreview(ui->listWidgetPhotos->currentRow());
            return true;
        }
        return false;
    }
    else
        return false;
}

void MainWindow::on_comboBoxSystems_currentIndexChanged(int index)
{
    return;

    if (index == -1)
    {
        return;
    }
    QAbstractItemModel* model = ui->comboBoxSystems->model();
    QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(model));
    standardModel->item(index)->setCheckState(Qt::Checked);
}

void MainWindow::on_lineEditProjectNo_textEdited(const QString &arg1)
{
    foreach (ProjectName name, _loader->GetProjectNames())
    {
        if (name.GetProjectNo().contains(arg1))
        {
            _backup.SetName(ui->lineEditProjectName->text());
            ui->lineEditProjectName->setText(name.GetName());
            return;
        }
    }
    ui->lineEditProjectName->setText(_backup.GetName());
}

void MainWindow::on_lineEditProjectName_textEdited(const QString &arg1)
{
    foreach (ProjectName name, _loader->GetProjectNames())
    {
        if (name.GetName().contains(arg1))
        {
            _backup.SetProjectNo(ui->lineEditProjectNo->text());
            ui->lineEditProjectNo->setText(name.GetProjectNo());
            return;
        }
    }
    ui->lineEditProjectNo->setText(_backup.GetProjectNo());
}
