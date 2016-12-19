#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "previewworker.h"
#include "interfacemanager.h"
#include "tableabstractelemmanager.h"
#include "attributeseditdialog.h"

#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>


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
    delete _searcher;
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
        _searcher = new PhotoSearcher(_dbm);
    }

    qDebug() << "db connect result " << result;
}

void MainWindow::InitInterface()
{
    ui->setupUi(this);
    ui->statusBar->setVisible(false);
    ui->menuBar->setVisible(false);

    setAcceptDrops(true);

    ui->pushButtonApplyFeature->setMinimumWidth(160);
    ui->pushButtonApplySystem->setMinimumWidth(160);
    ui->pushButtonNewFeature->setMinimumWidth(160);
    ui->pushButtonSystemsNew->setMinimumWidth(160);
    ui->dateEditProjectDate->setDateTime(QDateTime::currentDateTime());
    ui->dateEditProjectDateSearch->setDateTime(QDateTime(QDate(1970, 1, 1)));
    ui->dateEditProjectDateSearch->setEnabled(false);
    ui->dateEditPhotoIntervalBegin->setDateTime(QDateTime::currentDateTime());
    ui->dateEditPhotoIntervalEnd->setDateTime(QDateTime::currentDateTime());
    ui->listWidgetPhotos->setIconSize(QSize(0, 0));
    ui->listWidgetPhotos->installEventFilter(this);
    ui->tableWidgetPhotosSearch->setIconSize(QSize(0, 0));
    ui->tableWidgetPhotosSearch->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetPhotosSearch, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}

void MainWindow::LoadDatabase()
{
    if (_loader)
    {
        _loader->LoadDatabase();
    }

    if (_searcher)
    {
        _searcher->LoadDatabase();
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

    //create combobox with cheking elems for formworks
    InterfaceManager::FillCombobox(_searcher->GetFormworkSystems(), ui->comboBoxSystemsSearch);
    connect(ui->comboBoxSystemsSearch->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxSystemsSearch_ModelItemChanged(QStandardItem*)));

    //create combobox with cheking elems for features
    InterfaceManager::FillCombobox(_searcher->GetFeatures(), ui->comboBoxFeaturesSearch);
    connect(ui->comboBoxFeaturesSearch->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxFeaturesSearch_ModelItemChanged(QStandardItem*)));


    _selectedFeatures.clear();
    _selectedSystems.clear();

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
    QString projectNo = ui->lineEditProjectNo->text();
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

    QString projectName = ui->lineEditProjectName->text();
    _fileManager->CreateProjectDirectory(projectNo, projectName);
    _copierThread->setFileList(_files, _cfg->GetCompressionRate());
    _copierThread->start();
}



QString MainWindow::GetSelectedCategories()
{
    QVector<Categorie> selected;

    if (ui->checkBoxInProc->isChecked()) selected.push_back(_loader->GetCategories()[0]);
    if (ui->checkBoxCurrentState->isChecked()) selected.push_back(_loader->GetCategories()[1]);
    if (ui->checkBoxMarketing->isChecked()) selected.push_back(_loader->GetCategories()[2]);

    return TableAbstractElemManager::CreateIDsList(selected);
}

QVector<Categorie> MainWindow::GetSelectedCategoriesSearch()
{
    QVector<Categorie> selected;

    if (ui->checkBoxInProcSearch->isChecked()) selected.push_back(_searcher->GetCategories()[0]);
    if (ui->checkBoxCurrentStateSearch->isChecked()) selected.push_back(_searcher->GetCategories()[1]);
    if (ui->checkBoxMarketingSearch->isChecked()) selected.push_back(_searcher->GetCategories()[2]);

    return selected;
}

void MainWindow::ClearInterface(int tabIndex)
{
    if (tabIndex == 0)
    {
        ui->listWidgetPhotos->clear();
        ui->progressBar->setValue(0);
        _files.clear();
    }
    if (tabIndex == 1)
    {
        ui->tableWidgetPhotosSearch->clear();
        _searchResult.clear();;
    }
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

void MainWindow::on_comboBoxSystemsSearch_ModelItemChanged(QStandardItem *item)
{
    if (item->index().row() == 1)
    {
        InterfaceManager::ClearComboboxChecked(ui->comboBoxSystemsSearch);
    }
    else
    {
        if (item->index().row() == 0)
        {
            return;
        }

        _selectedSystems = InterfaceManager::GetSelectedListItems(_searcher->GetFormworkSystems(), ui->comboBoxSystemsSearch->model());
        InterfaceManager::ShowSelection(_selectedSystems, ui->comboBoxSystemsSearch);
    }
}

void MainWindow::on_comboBoxFeaturesSearch_ModelItemChanged(QStandardItem *item)
{
    if (item->index().row() == 1)
    {
        InterfaceManager::ClearComboboxChecked(ui->comboBoxFeaturesSearch);
    }
    else
    {
        if (item->index().row() == 0)
        {
            return;
        }

        _selectedFeatures = InterfaceManager::GetSelectedListItems(_searcher->GetFeatures(), ui->comboBoxFeaturesSearch->model());
        InterfaceManager::ShowSelection(_selectedFeatures, ui->comboBoxFeaturesSearch);
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
    if (index == 2)
    {
        InterfaceManager::FillTableWidget(_loader->GetFormworkSystems(), ui->tableWidgetSystems);
        InterfaceManager::FillTableWidget(_loader->GetFeatures(), ui->tableWidgetFeatures);

        _dbChangesFlag = false;
    }

    if ((index == 0) || (index == 1))
    {
        ui->tableWidgetSystems->clear();
        ui->tableWidgetFeatures->clear();
        LoadInterface();
    }
}


void MainWindow::on_pushButtonApplySystem_clicked()
{
    if (!ConfirmWindow())
        return;

    InterfaceManager::ApplyChanges(_loader->GetFormworkSystems(), ui->tableWidgetSystems);
    InterfaceManager::ApplyChanges(_searcher->GetFormworkSystems(), ui->tableWidgetSystems);
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
    InterfaceManager::ApplyChanges(_searcher->GetFeatures(), ui->tableWidgetFeatures);
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
    if ((index == 0) || (index == 1))
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

        InterfaceManager::ApplyChanges(_searcher->GetFormworkSystems(), ui->tableWidgetSystems);
        InterfaceManager::ApplyChanges(_searcher->GetFeatures(), ui->tableWidgetFeatures);

        LoadDatabase();
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
    QVector<FileInfoStruct> files = _copierThread->getCopiedFiles();


    QVector<QFileInfo> previews;
    for (int i = 0; i < _files.count(); i++)
    {
       previews.push_back(_fileManager->AddPreviewToDirectory(ui->listWidgetPhotos->item(i)->icon(), _files.at(i)));
    }


    QString projectNo = ui->lineEditProjectNo->text();
    QString projectName = ui->lineEditProjectName->text();
    QDate projectDate = ui->dateEditProjectDate->date();
    bool result = _loader->InsertToDatabase(projectNo, projectName, projectDate,
                                            _selectedSystems, _selectedFeatures, GetSelectedCategories(),
                                            files,
                                            previews);

    if (result)
    {
        QMessageBox::information(this, tr("Successfully"), tr("Photos were added to DB"));
        ClearInterface(0);
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
    return;

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
    return;

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

void MainWindow::on_pushButtonSearch_clicked()
{
    ClearInterface(1);

    QString projectNo = ui->lineEditProjectNoSearch->text();
    QString projectName = ui->lineEditProjectNameSearch->text();
    QDate projectDate = ui->dateEditProjectDateSearch->date();

    QVector<QFileInfo> previews;

    QDate intervalBegin = ui->dateEditPhotoIntervalBegin->date();
    QDate intervalEnd = ui->dateEditPhotoIntervalEnd->date();

    bool result = _searcher->SearchPhotos(projectNo, projectName, projectDate,
                                            _selectedSystems, _selectedFeatures, GetSelectedCategoriesSearch(),
                                            intervalBegin, intervalEnd,
                                            _searchResult);

    if (!result)
        return;

    ui->tableWidgetPhotosSearch->setColumnCount(3);
    ui->tableWidgetPhotosSearch->setRowCount(_searchResult.length());
    for (int i = 0; i < _searchResult.length(); i++)
    {
       SearchResult fnp = _searchResult[i];
       QString text = fnp.projectName.isEmpty() ? fnp.filePath : fnp.projectName;
       QTableWidgetItem* itemName = new QTableWidgetItem(text);
       QTableWidgetItem* itemImage = new QTableWidgetItem();
       QTableWidgetItem* itemDate = new QTableWidgetItem(fnp.photoDate.toString());

       if (!fnp.previewPath.isEmpty())
       {
           QPixmap pm(fnp.previewPath);
           itemImage->setIcon(QIcon(pm));
       }


       ui->tableWidgetPhotosSearch->setItem(i, 1, itemName);
       ui->tableWidgetPhotosSearch->setItem(i, 0, itemImage);
       ui->tableWidgetPhotosSearch->setItem(i, 2, itemDate);

    }
    ui->tableWidgetPhotosSearch->resizeColumnsToContents();
}

void MainWindow::on_pushButtonSavePhotos_clicked()
{
    QString destination = QDir::homePath() + "\\" + QStandardPaths::displayName(QStandardPaths::PicturesLocation) + "\\";
    QString saveDirectory = QFileDialog::getExistingDirectory(this, tr("Save images"), destination, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    bool result = true;
    /*QList<QListWidgetItem*> selected = ui->listWidgetPhotosSearch->selectedItems();

    if (selected.empty())
    {
        for (int i = 0; i < _searchResult.length(); i++)
        {
            QFileInfo fileInfo(_searchResult[i].filePath);
            QString destinationFileName = saveDirectory + "\\" + fileInfo.fileName();
            result &= QFile::copy(fileInfo.filePath(), destinationFileName);
        }
    }
    else
    {
        for (int i = 0; i < selected.length(); i++)
        {
            QListWidgetItem* item = selected[i];
            QFileInfo fileInfo(item->text());
            QString destinationFileName = saveDirectory + "\\" + fileInfo.fileName();
            result &= QFile::copy(fileInfo.filePath(), destinationFileName);
        }
    }

    if (result)
    {
        QMessageBox::information(this, tr("Successfully"), tr("Photos were copied to directory"));
        ClearInterface(1);
    }*/
}

void MainWindow::on_checkBoxDisableProjectDate_clicked()
{
    if (ui->checkBoxDisableProjectDate->isChecked())
    {
        ui->dateEditProjectDateSearch->setEnabled(false);
        ui->dateEditProjectDateSearch->setDateTime(QDateTime(QDate(1970, 1, 1)));
    }
    else
    {
        ui->dateEditProjectDateSearch->setEnabled(true);
        ui->dateEditProjectDateSearch->setDateTime(QDateTime::currentDateTime());
    }
}

void MainWindow::on_checkBoxDisableIntervalBegin_clicked()
{
    if (ui->checkBoxDisableIntervalBegin->isChecked())
    {
        ui->dateEditPhotoIntervalBegin->setEnabled(false);
        ui->dateEditPhotoIntervalBegin->setDateTime(QDateTime(QDate(1970, 1, 1)));
    }
    else
    {
        ui->dateEditPhotoIntervalBegin->setEnabled(true);
        ui->dateEditPhotoIntervalBegin->setDateTime(QDateTime::currentDateTime());
    }
}

void MainWindow::on_checkBoxDisableIntervalEnd_clicked()
{
    if (ui->checkBoxDisableIntervalEnd->isChecked())
    {
        ui->dateEditPhotoIntervalEnd->setEnabled(false);
        ui->dateEditPhotoIntervalEnd->setDateTime(QDateTime(QDate(1970, 1, 1)));
    }
    else
    {
        ui->dateEditPhotoIntervalEnd->setEnabled(true);
        ui->dateEditPhotoIntervalEnd->setDateTime(QDateTime::currentDateTime());
    }
}

void MainWindow::on_checkBoxEnablePreviewSearch_clicked()
{
    if (ui->checkBoxEnablePreviewSearch->isChecked())
    {
        //ui->tableWidgetPhotosSearch->setViewMode(QListWidget::IconMode);
        ui->tableWidgetPhotosSearch->setIconSize(QSize(100, 100));
        //ui->tableWidgetPhotosSearch->setResizeMode(QListWidget::Adjust);
    }
    else
    {
        //ui->listWidgetPhotosSearch->setViewMode(QListWidget::ListMode);
        ui->tableWidgetPhotosSearch->setIconSize(QSize(0, 0));
    }
}

void MainWindow::showContextMenu(QPoint pos)
{
    /*QListWidgetItem* item = ui->listWidgetPhotosSearch->itemAt(pos);
    if (item != nullptr)
    {
        QMenu contextMenu;
        QAction* save = contextMenu.addAction(tr("Save"));
        QAction* print = contextMenu.addAction(tr("Print"));
        QAction* edit = contextMenu.addAction(tr("Edit"));
        QAction* selectedAction = contextMenu.exec(ui->listWidgetPhotosSearch->mapToGlobal(pos));
        if (selectedAction)
        {
            if (selectedAction == save)
            {
                saveSelected(item);
            }

            if (selectedAction == print)
            {
                printSelected(item);
            }

            if (selectedAction == edit)
            {
                AttributesEditDialog* dlg = new AttributesEditDialog(this);
                int index = ui->listWidgetPhotosSearch->row(item);
                dlg->LoadSelected(_searcher, _searchResult[index].photoId);
                dlg->exec();
                delete dlg;
            }
        }
    }*/
}

void MainWindow::saveSelected(QListWidgetItem* item)
{
    if (item == nullptr)
    {
        return;
    }

    QString filePath = item->text();
    QString destination = QDir::homePath() + "\\" + QStandardPaths::displayName(QStandardPaths::PicturesLocation) + "\\";
    QString destinationFile = QFileDialog::getSaveFileName(this, tr("Save file"), destination, tr("Images (*.jpg)"));
    if (destinationFile != "")
    {
        bool result = QFile::copy(filePath, destinationFile);
        if (result)
        {
            QMessageBox::information(this, tr("Successfully"), tr("Photos were copied to directory"));
        }
    }
}

void MainWindow::printSelected(QListWidgetItem *item)
{
    try
    {
        if (item == nullptr)
        {
            return;
        }

        QString filePath = item->text();

        QPrinter printer(QPrinter::HighResolution);
        printer.setFullPage(true);
        printer.setPaperSize(QPrinter::A4);
        printer.setPageSize(QPrinter::A4);
        QPrintDialog *dlg = new QPrintDialog(&printer, this);
        if(dlg->exec() == QDialog::Accepted)
        {
            QPixmap img(filePath);
            QPainter painter(&printer);
            QRect rect = painter.viewport();
            QSize size = img.size();
            size.scale(rect.size(), Qt::KeepAspectRatio);
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(img.rect());
            painter.drawPixmap(0, 0, img);
        }

        delete dlg;
    }
    catch (QException& exp)
    {
        qDebug() << exp.what();
    }
}

void MainWindow::on_pushButtonPrintSelected_clicked()
{
    /*try
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setFullPage(true);
        printer.setPaperSize(QPrinter::A4);
        printer.setPageSize(QPrinter::A4);
        QPrintDialog *dlg = new QPrintDialog(&printer, this);

        if(dlg->exec() == QDialog::Accepted)
        {
            bool result = true;
            QList<QListWidgetItem*> selected = ui->listWidgetPhotosSearch->selectedItems();

            QPainter painter(&printer);
            if (selected.empty())
            {
                for (int i = 0; i < _searchResult.length(); i++)
                {
                    QPixmap pm(_searchResult[i].filePath);
                    painter.begin(&printer);
                    printer.newPage();
                    QRect rect = painter.viewport();
                    QSize size = pm.size();
                    size.scale(rect.size(), Qt::KeepAspectRatio);
                    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                    painter.setWindow(pm.rect());
                    painter.drawPixmap(0, 0, pm);
                    painter.end();
                }
            }
            else
            {
                for (int i = 0; i < selected.length(); i++)
                {
                    QListWidgetItem* item = selected[i];
                    QPixmap pm(item->text());
                    painter.begin(&printer);
                    printer.newPage();
                    //QPainter painter(&printer);
                    QRect rect = painter.viewport();
                    QSize size = pm.size();
                    size.scale(rect.size(), Qt::KeepAspectRatio);
                    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                    painter.setWindow(pm.rect());
                    painter.drawPixmap(0, 0, pm);
                    painter.end();
                }
            }
        }

        delete dlg;
    }
    catch (QException& exp)
    {
        qDebug() << exp.what();
    }*/
}

void MainWindow::on_checkBoxOrderByName_clicked()
{
}
