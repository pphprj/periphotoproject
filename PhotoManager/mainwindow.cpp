#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "previewworker.h"
#include "interfacemanager.h"
#include "tableabstractelemmanager.h"
#include "aboutwindow.h"


#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _cfg = new Configurator();
    _fileManager = new FileManager(_cfg->GetProjectsDirectory());

    InitDatabase();

    InitInterface();


    LoadDatabase();

    LoadInterface();
}

MainWindow::~MainWindow()
{
    delete _addPage;
    delete _searchPage;
    delete _editDBPage;

    delete _fileManager;
    delete _dbm;
    delete _cfg;
    delete ui;
}

void MainWindow::InitDatabase()
{
    _dbm = new DatabaseManager();
    bool result = _dbm->Connect(_cfg->GetHost(), _cfg->GetUsername(), _cfg->GetPassword());

    qDebug() << "db connect result " << result;
}

void MainWindow::InitInterface()
{
    ui->setupUi(this);
    ui->statusBar->setVisible(false);

    setAcceptDrops(true);

    ui->pushButtonApplyFeature->setMinimumWidth(160);
    ui->pushButtonApplySystem->setMinimumWidth(160);
    ui->pushButtonNewFeature->setMinimumWidth(160);
    ui->pushButtonSystemsNew->setMinimumWidth(160);
    ui->dateEditProjectDate->setDateTime(QDateTime::currentDateTime());
    ui->dateEditProjectDateSearch->setDateTime(QDateTime(QDate(1970, 1, 1)));
    ui->dateEditProjectDateSearch->setEnabled(false);
    ui->dateEditPhotoIntervalBegin->setDateTime(QDateTime(QDate(1970, 1, 1)));
    ui->dateEditPhotoIntervalBegin->setEnabled(false);
    ui->dateEditPhotoIntervalEnd->setDateTime(QDateTime(QDate(1970, 1, 1)));
    ui->dateEditOnly->setDateTime(QDateTime::currentDateTime());
    ui->dateEditPhotoIntervalEnd->setEnabled(false);
    ui->dateEditOnly->setEnabled(false);
    ui->listWidgetPhotos->setIconSize(QSize(0, 0));
    ui->listWidgetPhotos->installEventFilter(this);

    if (!_cfg->ShowEditDBTab())
    {
        ui->tabWidgetSystem->removeTab(2);
    }

    if (!_cfg->ShowDeleteButton())
    {
        ui->pushButtonDeleteSelected->setVisible(false);
    }

    ui->tableWidgetSearchResult->setIconSize(QSize(100, 100));
    ui->tableWidgetSearchResult->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetSearchResult->setColumnCount(9);
    QStringList headers;
    headers << tr("Preview")
            << tr("Photo date")
            << tr("Project No")
            << tr("Project name / File path")
            << tr("Company")
            << tr("Formwork systems")
            << tr("Features")
            << tr("Description")
            << "";
    ui->tableWidgetSearchResult->setHorizontalHeaderLabels(headers);
    ui->tableWidgetSearchResult->resizeColumnsToContents();
    ui->tableWidgetSearchResult->setColumnHidden(8, true);


    _addPage = new AdditionPage(_dbm, _fileManager, _cfg, this);
    QVector<QCheckBox*> cbxs;
    cbxs.push_back(ui->checkBoxInProc);
    cbxs.push_back(ui->checkBoxCurrentState);
    cbxs.push_back(ui->checkBoxMarketing);
    _addPage->InitInterface(ui->lineEditProjectNo, ui->lineEditProjectName, ui->dateEditProjectDate,
                            ui->lineEditCompanyName, ui->plainTextEditDescription,
                            ui->comboBoxSystems, ui->comboBoxFeatures, cbxs);
    _addPage->InitAdditionPageInterface(ui->listWidgetPhotos, ui->progressBar);
    _addPage->CreateLoader();

    _searchPage = new SearchPage(_dbm, _fileManager, _cfg, this);
    cbxs.clear();
    cbxs.push_back(ui->checkBoxInProcSearch);
    cbxs.push_back(ui->checkBoxCurrentStateSearch);
    cbxs.push_back(ui->checkBoxMarketingSearch);
    _searchPage->InitInterface(ui->lineEditProjectNoSearch, ui->lineEditProjectNameSearch, ui->dateEditProjectDateSearch,
                               ui->lineEditCompanyNameSearch, nullptr,
                               ui->comboBoxSystemsSearch, ui->comboBoxFeaturesSearch, cbxs);
    cbxs.clear();
    cbxs.push_back(ui->checkBoxInProgressFilters);
    cbxs.push_back(ui->checkBoxCurrentStateFilters);
    cbxs.push_back(ui->checkBoxMarketingFilters);
    _searchPage->InitSearchPageInterface(ui->dateEditPhotoIntervalBegin, ui->dateEditPhotoIntervalEnd, ui->tableWidgetSearchResult,
                                         cbxs, ui->checkBoxFilterByDate, ui->dateEditOnly);
    _searchPage->CreateLoader();

    _editDBPage = new EditDBPage(_dbm, _cfg, this);
    _editDBPage->InitInterface(ui->tableWidgetSystems, ui->tableWidgetFeatures);
}

void MainWindow::LoadDatabase()
{
    _addPage->LoadDatabase();
    _searchPage->LoadDatabase();
    _editDBPage->LoadDatabase();
}

void MainWindow::LoadInterface()
{
    _addPage->FillInterface();
    _searchPage->FillInterface();

    ui->checkBoxEnablePreview->setChecked(true);
    on_checkBoxEnablePreview_clicked();
}


void MainWindow::on_pushButtonLoadPhoto_clicked()
{
    QFileDialog fileDialog(this, tr("Open images"), _cfg->GetLastFolder(), tr("Images (*.jpg *.jpeg);;Documents (*.pdf)"));
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
            _addPage->AddFileToPreview(str);
        }
    }
}

void MainWindow::on_pushButtonAddToDB_clicked()
{
    _addPage->AddFilesToDatabase();
}


void MainWindow::ClearInterface(int tabIndex)
{
    if (tabIndex == 0)
    {
        _addPage->ClearInterface();
    }
    if (tabIndex == 1)
    {
        _searchPage->ClearInterface();
    }
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
            (event->mimeData()->text().contains(".jpeg", Qt::CaseInsensitive)) ||
            (event->mimeData()->text().contains(".pdf", Qt::CaseInsensitive))
           )
        {
            if (event->mimeData()->hasUrls())
            {
                QList<QUrl> urlList = event->mimeData()->urls();

                // extract the local paths of the files
                for (int i = 0; i < urlList.size(); i++)
                {
                    QString fileName = urlList.at(i).toLocalFile();
                    _addPage->AddFileToPreview(fileName);
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
        _editDBPage->FillInterface();
    }

    if ((index == 0) || (index == 1))
    {
        _editDBPage->ClearInterface();
    }
}


void MainWindow::on_pushButtonApplySystem_clicked()
{
    if (!ConfirmWindow())
        return;

    _editDBPage->UpdateFormworkSystems();

    LoadDatabase();
    LoadInterface();
}

void MainWindow::on_pushButtonSystemsNew_clicked()
{
    _editDBPage->NewSystem();
}

void MainWindow::on_pushButtonNewFeature_clicked()
{
    _editDBPage->NewFeature();
}

void MainWindow::on_pushButtonApplyFeature_clicked()
{
    if (!ConfirmWindow())
        return;

    _editDBPage->UpdateFeatures();

    LoadDatabase();
    LoadInterface();
}


void MainWindow::on_tabWidgetSystem_tabBarClicked(int index)
{
    if ((index == 0) || (index == 1))
    {
        if (ui->tabWidgetSystem->currentIndex() == index)
            return;

        if (!_editDBPage->DBWasChanged())
            return;

        if (!ConfirmWindow())
            return;

        _editDBPage->UpdateFormworkSystems();
        _editDBPage->UpdateFeatures();


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

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->listWidgetPhotos && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete)
        {
            //DeleteFileFromPreview());
            _addPage->DeleteFileFromPreview(ui->listWidgetPhotos->currentRow());
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

void MainWindow::on_pushButtonSearch_clicked()
{
    ClearInterface(1);

    _searchPage->SearchPhotos();
}

void MainWindow::on_pushButtonSavePhotos_clicked()
{
   _searchPage->SavePhotos();
}

void MainWindow::on_checkBoxDisableProjectDate_clicked()
{
    InterfaceManager::EnableDateEditField(ui->dateEditProjectDateSearch,
                                          ui->checkBoxDisableProjectDate->isChecked());
}

void MainWindow::on_checkBoxDisableIntervalBegin_clicked()
{
    InterfaceManager::EnableDateEditField(ui->dateEditPhotoIntervalBegin,
                                          ui->checkBoxDisableIntervalBegin->isChecked());
}

void MainWindow::on_checkBoxDisableIntervalEnd_clicked()
{
    InterfaceManager::EnableDateEditField(ui->dateEditPhotoIntervalEnd,
                                          ui->checkBoxDisableIntervalEnd->isChecked());
}


void MainWindow::on_pushButtonPrintSelected_clicked()
{
   _searchPage->PrintPhotos();
}

void MainWindow::on_checkBoxOrderByName_clicked()
{
    ui->tableWidgetSearchResult->sortItems(3);
}

void MainWindow::on_dateEditOnly_dateChanged(const QDate &date)
{
    if (!ui->checkBoxFilterByDate->isChecked()) return;

    _searchPage->OrderDateOnly(date);
}

void MainWindow::on_checkBoxFilterByDate_clicked()
{
   _searchPage->OrderByDate();
}

void MainWindow::on_tableWidgetPhotosSearch_doubleClicked(const QModelIndex &index)
{
    QTableWidgetItem* item = ui->tableWidgetSearchResult->item(index.row(), index.column());
    _searchPage->ShowItemPreview(item);
}

void MainWindow::on_actionE_xit_triggered()
{
    this->close();
}

void MainWindow::on_action_About_triggered()
{
    AboutWindow* about = new AboutWindow(this);
    about->exec();
    delete about;
}

void MainWindow::on_groupBoxCategoriesSearch_toggled(bool arg1)
{
    if (!arg1)
    {
        ui->checkBoxCurrentStateSearch->setVisible(false);
        ui->checkBoxInProcSearch->setVisible(false);
        ui->checkBoxMarketingSearch->setVisible(false);
    }
    else
    {
        ui->checkBoxCurrentStateSearch->setVisible(true);
        ui->checkBoxInProcSearch->setVisible(true);
        ui->checkBoxMarketingSearch->setVisible(true);
    }
}

void MainWindow::on_groupBoxProjectSearch_toggled(bool arg1)
{
    on_groupBoxCategoriesSearch_toggled(arg1);

    if (!arg1)
    {
        for (int i = 0; i < ui->gridLayout_6->count(); i++)
        {
            QWidget* widget = ui->gridLayout_6->itemAt(i)->widget();
            if (widget != nullptr)
            {
                widget->setVisible(false);
            }
        }
    }
    else
    {
        for (int i = 0; i < ui->gridLayout_6->count(); i++)
        {
            QWidget* widget = ui->gridLayout_6->itemAt(i)->widget();
            if (widget != nullptr)
            {
                widget->setVisible(true);
            }
        }
    }
}

void MainWindow::on_pushButtonDeleteSelected_clicked()
{
    _searchPage->DeletePhotos();
}

void MainWindow::refreshProjectsSlot()
{
    _searchPage->RefreshProjects();
}


void MainWindow::on_tableWidgetSearchResult_doubleClicked(const QModelIndex &index)
{
    QTableWidgetItem* item = ui->tableWidgetSearchResult->item(index.row(), index.column());
    _searchPage->ShowItemPreview(item);
}
