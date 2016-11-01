#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    QObject::connect(_copierThread, SIGNAL(progress(int)), this, SLOT(setProgressBarValue(int)));
    QObject::connect(_copierThread, SIGNAL(finished()), this, SLOT(finishedCopy()));

    _dbChangesFlag = false;
}

MainWindow::~MainWindow()
{
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
    if(_dbm)
    {
        _formworkSystems.clear();
        _dbm->SelectFormworkSystems(_formworkSystems);
        _features.clear();
        _dbm->SelectFeatures(_features);
        _categories.clear();
        _dbm->SelectCategories(_categories);
        _projectNames.clear();
        _dbm->SelectProjectNames(_projectNames);
    }
}

void MainWindow::LoadInterface()
{
    //create combobox with cheking elems for formworks
    FillCombobox(_formworkSystems, ui->comboBoxSystems);
    connect(ui->comboBoxSystems->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxSystems_ModelItemChanged(QStandardItem*)));

    //create combobox with cheking elems for features
    FillCombobox(_features, ui->comboBoxFeatures);
    connect(ui->comboBoxFeatures->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_comboBoxFeatures_ModelItemChanged(QStandardItem*)));
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

        QPixmap pm(fileName);
        QPixmap scaled = pm.scaled(400, 300, Qt::KeepAspectRatio).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        ui->listWidgetPhotos->addItem(new QListWidgetItem(QIcon(scaled), fileName));
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

    QVector<FormworkSystem> selectedSystems = GetSelectedListItems(_formworkSystems, ui->comboBoxSystems->model());
    QString selectedFws = CreateIDsList(selectedSystems);
    if (selectedFws.isEmpty())
    {
        QMessageBox::critical(this, tr("Error!"), tr("Please, select formworks!"));
        return;
    }

    QVector<Feature> selectedFeatures = GetSelectedListItems(_features, ui->comboBoxFeatures->model());
    QString selectedFts = CreateIDsList(selectedFeatures);
    if (selectedFts.isEmpty())
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

    _fileManager->CreateProjectDirectory(projectNo);
    _copierThread->setFileList(GetFileList(), _cfg->GetCompressionRate());
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
    return;
   /* for (int i = 0; i < _files.length(); i++)
    {
        QFileInfo info(_files[i]);
        QDate selectedDate = GetProjectDate();
        QDate lastModified = info.lastModified().date();
        if (selectedDate != lastModified)
        {
            if ((_filesDate != lastModified) && (!_filesDate.isNull()))
            {
                _filesDate = selectedDate;
            }
            else
            {
                _filesDate = lastModified;
            }
        }
        else
        {
            _filesDate = selectedDate;
        }
    }*/
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

template <typename T> QString MainWindow::CreateNamesList(const QVector<T>& elems)
{
    QString result;

    for(int i = 0; i < elems.length(); i++)
    {
        T elem = elems[i];
        result += elem.GetName() + ";";
    }

    result.remove(result.length() - 1, 1);

    return result;
}

template <typename T> void MainWindow::FillCombobox(const QVector<T>& elems, const QComboBox* comboBox)
{
    QComboBox* cb = const_cast<QComboBox*>(comboBox);

    cb->clear();

    QStandardItemModel* model = new QStandardItemModel(elems.length() + 2, 1);

    QStandardItem* itemShowSelecatble = new QStandardItem("");
    model->setItem(0, 0, itemShowSelecatble);


    QStandardItem* itemDisableAll = new QStandardItem(tr("Disable all"));

    itemDisableAll->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    itemDisableAll->setData(Qt::Unchecked, Qt::CheckStateRole);

    model->setItem(1, 0, itemDisableAll);

    for (int i = 2; i < elems.length() + 2; i++)
    {
        T elem = elems[i - 2];
        QStandardItem* item = new QStandardItem(elem.GetName());

        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);

        model->setItem(i, 0, item);
    }


    cb->setModel(model);
    cb->setCurrentIndex(0);
}

template <typename T> void MainWindow::FillTableWidget(const QVector<T>& elems, const QTableWidget* table)
{
    QTableWidget* tbWidget = const_cast<QTableWidget*>(table);
    tbWidget->setRowCount(elems.length());

    tbWidget->setColumnCount(2);
    for (int i = 0; i < elems.length(); i++)
    {
        T elem = elems[i];
        QTableWidgetItem* itemName = new QTableWidgetItem(elem.GetName());
        itemName->setFlags(itemName->flags() | Qt::ItemIsEditable);

        QString str = elem.GetDesription();
        QTableWidgetItem* itemDescription = new QTableWidgetItem(str);
        itemDescription->setFlags(itemDescription->flags() | Qt::ItemIsEditable);

        tbWidget->setItem(i, 0, itemName);
        tbWidget->setItem(i, 1, itemDescription);
    }
    tbWidget->resizeColumnsToContents();
}

template <typename T> QVector<T> MainWindow::GetSelectedListItems(const QVector<T>& elems, const QAbstractItemModel *model)
{
    QVector<T> selected;
    QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(model));
    int length = standardModel->rowCount();
    for (int i = 2; i < length; i++)
    {
        if (standardModel->item(i)->checkState() == Qt::Checked)
        {
            selected.push_back(elems[i - 2]);
        }
    }
    return selected;
}

template <typename T> void MainWindow::ApplyChanges(QVector<T>& elems, const QTableWidget* table)
{
    QTableWidget* tbWidget = const_cast<QTableWidget*>(table);
    for (int i = 0; i < tbWidget->rowCount(); i++)
    {
       QTableWidgetItem* name = tbWidget->item(i, 0);
       QTableWidgetItem* descr = tbWidget->item(i, 1);

       if (name == nullptr)
       {
           continue;
       }

       QString nameText = name->text();
       QString descrText = (descr != nullptr) ? descr->text() : "";

       if (i < elems.length())
       {
           elems[i].SetName(nameText);
           elems[i].SetDescription(descrText);
       }
       else
       {
           if (nameText != tr("New item"))
           {
               elems.push_back(T(0, nameText, descrText));
           }
       }
    }
    _dbChangesFlag = false;
}

template <typename T> void MainWindow::ItemChanged(QVector<T>& elems, QTableWidgetItem* item, QTableWidget* table)
{
    if (item->text().isEmpty() && item->column() == 0)
    {
        if (item->row() < elems.length())
        {
            item->setText(elems[item->row()].GetName());
        }
        else
        {
            //if you no enter new name
            table->removeRow(item->row());
        }
    }
    _dbChangesFlag = true;
}

void MainWindow::NewItem( QTableWidget* table)
{
    QTableWidgetItem* item = new QTableWidgetItem(tr("New item"));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    table->insertRow(table->rowCount());
    table->setItem(table->rowCount() - 1, 0, item);
    table->setCurrentItem(item);
    _dbChangesFlag = true;
}

template <typename T> void MainWindow::ShowSelection(const QVector<T>& elems, QComboBox *comboBox, QStandardItem *item)
{
    if (item->index().row() == 0)
    {
        return;
    }

    QVector<T> selected = GetSelectedListItems(elems, comboBox->model());
    QString selectedList = CreateNamesList(selected);
    comboBox->setItemText(0, selectedList);
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

void MainWindow::ClearComboboxChecked(QComboBox* comboBox)
{
    QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(comboBox->model());
    for (int i = 0; i < standardModel->rowCount(); i++)
    {
        if (standardModel->item(i)->checkState() == Qt::Checked)
        {
            standardModel->item(i)->setCheckState(Qt::Unchecked);
        }
    }
    comboBox->setItemText(0, "");
}

void MainWindow::ClearInterface()
{
    ui->listWidgetPhotos->clear();
    ui->progressBar->setValue(0);
    _files.clear();
    //ClearComboboxChecked(ui->comboBoxFeatures->model());
    //ClearComboboxChecked(ui->comboBoxSystems->model());
}

void MainWindow::on_comboBoxSystems_ModelItemChanged(QStandardItem *item)
{
    if (item->index().row() == 1)
    {
        ClearComboboxChecked(ui->comboBoxSystems);
    }
    else
    {
        ShowSelection(_formworkSystems, ui->comboBoxSystems, item);
    }
}

void MainWindow::on_comboBoxFeatures_ModelItemChanged(QStandardItem *item)
{
    if (item->index().row() == 1)
    {
        ClearComboboxChecked(ui->comboBoxFeatures);
    }
    else
    {
        ShowSelection(_features, ui->comboBoxFeatures, item);
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
        FillTableWidget(_formworkSystems, ui->tableWidgetSystems);
        FillTableWidget(_features, ui->tableWidgetFeatures);

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

    ApplyChanges(_formworkSystems, ui->tableWidgetSystems);

    _dbm->UpdateFormworkSystems(_formworkSystems);

    LoadDatabase();
    LoadInterface();
}

void MainWindow::on_tableWidgetSystems_itemChanged(QTableWidgetItem *item)
{
    ItemChanged(_formworkSystems, item, ui->tableWidgetSystems);
}

void MainWindow::on_pushButtonSystemsNew_clicked()
{
    NewItem(ui->tableWidgetSystems);
}

void MainWindow::on_pushButtonNewFeature_clicked()
{
    NewItem(ui->tableWidgetFeatures);
}

void MainWindow::on_pushButtonApplyFeature_clicked()
{
    if (!ConfirmWindow())
        return;

    ApplyChanges(_features, ui->tableWidgetFeatures);
    _dbm->UpdateFeatures(_features);
    LoadDatabase();
    LoadInterface();
}

void MainWindow::on_tableWidgetFeatures_itemChanged(QTableWidgetItem *item)
{
    ItemChanged(_features, item, ui->tableWidgetFeatures);
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

        ApplyChanges(_formworkSystems, ui->tableWidgetSystems);
        _dbm->UpdateFormworkSystems(_formworkSystems);

        ApplyChanges(_features, ui->tableWidgetFeatures);
        _dbm->UpdateFeatures(_features);

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
    ;
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

    QString projectName = GetProjectName();
    QString projectNo = GetProjectNo();

    QVector<FormworkSystem> selectedSystems = GetSelectedListItems(_formworkSystems, ui->comboBoxSystems->model());
    QString selectedFws = CreateIDsList(selectedSystems);

    QVector<Feature> selectedFeatures = GetSelectedListItems(_features, ui->comboBoxFeatures->model());
    QString selectedFts = CreateIDsList(selectedFeatures);

    QString selectedCategories = GetSelectedCategories();


    bool result = _dbm->InsertValuesToPhotos(projectNo,
                                             projectName,
                                             GetProjectDate(),
                                             selectedFws,
                               selectedFts,
                               selectedCategories,
                               files);

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
    QAbstractItemModel* model = ui->comboBoxSystems->model();
    QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(model));
    standardModel->item(index)->setCheckState(Qt::Checked);
}

void MainWindow::on_lineEditProjectNo_textEdited(const QString &arg1)
{
    foreach (ProjectName name, _projectNames)
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
    foreach (ProjectName name, _projectNames)
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
