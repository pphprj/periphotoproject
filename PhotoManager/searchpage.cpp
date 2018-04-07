#include "searchpage.h"

#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QProcessEnvironment>

#include <qt_windows.h>

#include "resultcontextmenu.h"
#include "attributeseditdialog.h"
#include "confirmdeletedialog.h"
#include "imageprinter.h"
#include "tableabstractelemmanager.h"
#include "bigpreview.h"

SearchPage::SearchPage(DatabaseManager *databaseManager, FileManager *fileManager, Configurator *cfg, QObject *parent)
    : Page(databaseManager, fileManager, cfg, parent)
{
    connect(databaseManager, SIGNAL(AddSearchResultToTable(int,SearchResult)), this, SLOT(addSearchResultToTable(int,SearchResult)));
    connect(databaseManager, SIGNAL(FinishSearch()), this, SLOT(finishSearch()));
}

SearchPage::~SearchPage()
{
}

void SearchPage::CreateLoader()
{
    _searcher = new PhotoSearcher(_dbm);
    _loader = _searcher;
}

void SearchPage::ClearInterface()
{
    _tableResults->clear();
    _searchResult.clear();
}

void SearchPage::InitSearchPageInterface(QDateEdit *beginInterval, QDateEdit *endInterval, QTableWidget *tableResults,
                                         const QVector<QCheckBox *> &filterCategories, QCheckBox* filterByDate, QDateEdit* filterDate)
{
    _beginInterval = beginInterval;
    _endInterval = endInterval;
    _tableResults = tableResults;

    foreach (QCheckBox* cb, filterCategories)
    {
        _filterCategories.push_back(cb);
        connect(cb, SIGNAL(clicked()), this, SLOT(checkBoxClickedSlot()));
    }

    _filterByDate = filterByDate;
    _filterDate = filterDate;

    connect(_tableResults, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(_tableResults->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionClickedSlot(int)));

    qDebug() << "Init search page";
}

void SearchPage::SearchPhotos()
{
    ClearInterface();

    QString projectNo = _projectNo->text();
    QString projectName = _projectName->text();
    QDate projectDate = _projectDate->date();
    QString company = _companyName->text();
    QString address = _address->text();

    QDate intervalBegin = _beginInterval->date();
    QDate intervalEnd = _endInterval->date();

    fillSearchResultTable(0);

    bool result = _searcher->SearchPhotos(projectNo, projectName, projectDate, company, address,
                                            _selectedSystems, _selectedFeatures, GetSelectedCategories(),
                                            intervalBegin, intervalEnd,
                                            _searchResult);

    if (!result)
    {
        qDebug() << "Search was crashed";
        return;
    }

    qDebug() << "Search result " << _searchResult.length();

    QHeaderView* header = _tableResults->verticalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setDefaultSectionSize(100);
    _tableResults->resizeColumnsToContents();
    _tableResults->setColumnHidden(0, true);
}

void SearchPage::SavePhotos()
{
    QString destination = QDir::homePath() + "\\" + QStandardPaths::displayName(QStandardPaths::PicturesLocation) + "\\";
    QString saveDirectory = QFileDialog::getExistingDirectory(nullptr, tr("Save images"), destination, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    bool result = true;
    QList<QTableWidgetItem*> selected = _tableResults->selectedItems();

    if (selected.empty())
    {
        for (int i = 0; i < _searchResult.length(); i++)
        {
            result &= _fileManager->SaveFileToDirectory(_searchResult[i].filePath, saveDirectory);
        }
    }
    else
    {
        for (int i = 0; i < selected.length(); i++)
        {
            QTableWidgetItem* item = selected[i];
            int index = GetSearchResultIndexByItem(item);
            qDebug() << "Save Index " << index;
            if (index != -1)
            {
                SearchResult res = _searchResult[index];
                result &= _fileManager->SaveFileToDirectory(res.filePath, saveDirectory);
            }
        }
    }

    if (result)
    {
        QMessageBox::information(nullptr, tr("Successfully"), tr("Photos were copied to directory"));
    }
}

void SearchPage::PrintPhotos()
{
    try
    {
        ImagePrinter* printer = new ImagePrinter();

        if(printer->ShowDialog())
        {
            QList<QTableWidgetItem*> selected = _tableResults->selectedItems();

            if (selected.empty())
            {
                for (int i = 0; i < _searchResult.length(); i++)
                {
                    printer->PrintImage(_searchResult[i].filePath);
                }
            }
            else
            {
                for (int i = 0; i < selected.length(); i++)
                {
                    QTableWidgetItem* item = selected[i];
                    int index = GetSearchResultIndexByItem(item);
                    qDebug() << "Print Index " << index;
                    if (index != -1)
                    {
                        SearchResult res = _searchResult[index];
                        printer->PrintImage(res.filePath);
                    }
                }
            }
        }

        delete printer;
    }
    catch (QException& exp)
    {
        qDebug() << exp.what();
    }
}

void SearchPage::DeletePhotos()
{
    ConfirmDeleteDialog confirm(_cfg->GetDeletePassword());
    if (!confirm.exec())
    {
        return;
    }

    QList<QTableWidgetItem*> selected = _tableResults->selectedItems();

    if (selected.empty())
    {
        for (int i = 0; i < _tableResults->rowCount(); i++)
        {
            QTableWidgetItem* item = _tableResults->item(i, 0);
            RemoveItem(item);
        }
    }
    else
    {
        for (int i = selected.count() - 1; i >= 0; i--)
        {
            QTableWidgetItem* item = selected[i];
            RemoveItem(item);
        }
    }

    SearchPhotos();
}

void SearchPage::RefreshProjects()
{
    _searcher->RefreshProjectNames();
}

void SearchPage::RemoveItem(QTableWidgetItem *item)
{
    if (item == nullptr)
    {
        return;
    }

    int index = GetSearchResultIndexByItem(item);
    qDebug() << "Remove Index " << index;
    QString filePath = _searchResult[index].filePath;
    _fileManager->DeleteFileFromDirectory(filePath);
    QString previewPath = _searchResult[index].previewPath;
    _fileManager->DeleteFileFromDirectory(previewPath);

    _dbm->DeletePhoto(_searchResult[index].photoId);
    _dbm->DeletePreview(_searchResult[index].photoId);

    _tableResults->removeRow(index);

    _searchResult.remove(index);
}


void SearchPage::OrderDateOnly(const QDate &date)
{
    QDate orderDate = date;
    for (int i = 0; i < _searchResult.length(); i++)
    {
        if (orderDate != _searchResult[i].photoDate)
        {
            QTableWidgetItem* item = GetItemByIndex(i);
            _tableResults->setRowHidden(item->row(), true);
        }
        else
        {
            QTableWidgetItem* item = GetItemByIndex(i);
            _tableResults->setRowHidden(item->row(), false);
        }
    }
}

void SearchPage::OrderByDate()
{
    if (_filterByDate->isChecked())
    {
        _filterDate->setEnabled(true);
        OrderDateOnly(_filterDate->date());
    }
    else
    {
        _filterDate->setEnabled(false);
        for (int i = 0; i < _searchResult.length(); i++)
        {
            _tableResults->setRowHidden(i, false);
        }
    }
}

void SearchPage::ShowItemPreview(QTableWidgetItem* item)
{
    if (item == nullptr) return;

    int index = GetSearchResultIndexByItem(item);
    qDebug() << "Show Index " << index;
    SearchResult searchItem = _searchResult[index];
    QFileInfo info(searchItem.filePath);
    if (info.suffix() == "pdf")
    {
        //QDesktopServices::openUrl(QUrl::fromLocalFile(item.filePath));
    }
    else
    {
        BigPreview* preview = new BigPreview(searchItem.filePath);
        preview->exec();
        delete preview;
    }
}

int SearchPage::GetSearchResultIndexByItem(QTableWidgetItem *item)
{
    if (item == nullptr)
        return -1;

    QString index = _tableResults->item(item->row(), 0)->text();
    return index.toInt();
}

QTableWidgetItem* SearchPage::GetItemByIndex(int index)
{
    QTableWidgetItem* result = nullptr;
    for (int i = 0; i < _tableResults->rowCount(); i++)
    {
        result = _tableResults->item(i, 0);
        if (index == result->text().toInt())
        {
            break;
        }
    }
    return result;
}

void SearchPage::showContextMenu(QPoint pos)
{
    ResultContextMenu contexMenu(pos,
                                 _tableResults,
                                 _cfg->ShowDeleteButton(),
                                 this);
    contexMenu.Execute();
}

void SearchPage::saveSelected(QTableWidgetItem* item)
{
    if (item == nullptr)
    {
        return;
    }

    int index = GetSearchResultIndexByItem(item);
    qDebug() << "Save Index " << index;
    QString filePath = _searchResult[index].filePath;
    QString destination = QDir::homePath() + "\\" + QStandardPaths::displayName(QStandardPaths::PicturesLocation) + "\\";
    QString destinationFile = QFileDialog::getSaveFileName(nullptr, tr("Save file"), destination, tr("Images (*.jpg *.jpeg);;Documents (*.pdf)"));
    if (destinationFile != "")
    {
        bool result = QFile::copy(filePath, destinationFile);
        if (result)
        {
            QMessageBox::information(nullptr, tr("Successfully"), tr("Photos were copied to directory"));
        }
    }
}

void SearchPage::printSelected(QTableWidgetItem *item)
{
    try
    {
        if (item == nullptr)
        {
            return;
        }

        int index = GetSearchResultIndexByItem(item);
        qDebug() << "Print Index " << index;
        QString filePath = _searchResult[index].filePath;
        ImagePrinter* printer = new ImagePrinter();

        if(printer->ShowDialog())
        {
            printer->PrintImage(filePath);
        }

        delete printer;
    }
    catch (QException& exp)
    {
        qDebug() << exp.what();
    }
}

void SearchPage::editSelected(QTableWidgetItem *item)
{
    AttributesEditDialog* dlg = new AttributesEditDialog();
    int index = GetSearchResultIndexByItem(item);
    qDebug() << "Edit Index " << index;
    dlg->LoadSelected(_searcher, _searchResult[index].photoId);
    if (dlg->exec() == QDialog::Accepted)
    {
        SearchPhotos();
    }
    delete dlg;
}

void SearchPage::removeSelected(QTableWidgetItem *item)
{
    ConfirmDeleteDialog confirm(_cfg->GetDeletePassword());
    if (confirm.exec())
    {
        RemoveItem(item);
        SearchPhotos();
    }
}

void SearchPage::showSelected(QTableWidgetItem *item)
{
    if (item == nullptr)
    {
        return;
    }

    try
    {
        int index = GetSearchResultIndexByItem(item);
        if (index == -1)
        {
            return;
        }
        qDebug() << "Show Index " << index;

        const QString explorer = QLatin1String("explorer.exe");
        QString param = QLatin1String("/select,");
        QString filePath = _searchResult[index].filePath;

        qDebug() << "File path before " << filePath;

        //delete all duplicates //
        while (filePath.indexOf("//") != -1)
        {
            filePath = filePath.replace(QString("//"), QString("/"));
        }

        qDebug() << "File path after " << filePath;

        if (filePath[0] == '/')
        {
            filePath = "/" + filePath;
        }

        filePath = "\"" + filePath + "\"";

        param += QDir::toNativeSeparators(filePath);
        QString command = explorer + " " + param;
        qDebug() << "Show command " << command;

        ShellExecute(0, L"open", L"explorer.exe", param.toStdWString().c_str(), 0, SW_NORMAL);
    }
    catch (QException& exp)
    {
        qDebug() << exp.what();
    }


}

void SearchPage::sectionClickedSlot(int column)
{
    Qt::SortOrder sortOrder = _tableResults->horizontalHeader()->sortIndicatorOrder();
    _tableResults->sortByColumn(column, sortOrder);
}

void SearchPage::checkBoxClickedSlot()
{
    QString search = "";

    for (int i = 0; i < _filterCategories.length(); i++)
    {
        if (_filterCategories[i]->isChecked())
        {
            if (!search.isEmpty()) search += ";";
            search += QString::number(i + 1);
        }
    }

    for (int i = 0; i < _searchResult.length(); i++)
    {
        if (_searchResult[i].categories.indexOf(search) == -1)
        {
            QTableWidgetItem* item = GetItemByIndex(i);
            _tableResults->setRowHidden(item->row(), true);
        }
        else
        {
            QTableWidgetItem* item = GetItemByIndex(i);
            _tableResults->setRowHidden(item->row(), false);
        }
    }
}

void SearchPage::addSearchResultToTable(int rowIndex, SearchResult fnp)
{
   // qDebug() << fnp;
    _tableResults->insertRow(rowIndex);

    QTableWidgetItem* itemProjecNo = new QTableWidgetItem(fnp.projectNo);

    QString text = fnp.projectName;
    QTableWidgetItem* itemName = new QTableWidgetItem(text);

    QTableWidgetItem* itemImage = new QTableWidgetItem();
    if (!fnp.previewPath.isEmpty())
    {
        itemImage->setSizeHint(QSize(100, 100));
        QPixmap pm(fnp.previewPath);
        itemImage->setData(Qt::DecorationRole, pm);
    }

    QTableWidgetItem* itemDate = new QTableWidgetItem(fnp.photoDate.toString("yyyy-MM-dd"));

    QVector<FormworkSystem> selectedFormworks;
    _searcher->SelectedFormworkSystems(selectedFormworks, fnp.formworksSystems);
    QString formworks = TableAbstractElemManager::CreateNamesList(selectedFormworks);
    QTableWidgetItem* itemFormworks = new QTableWidgetItem(formworks);


    QVector<Feature> selectedFeatures;
    _searcher->SelectedFeatures(selectedFeatures, fnp.features);
    QString features = TableAbstractElemManager::CreateNamesList(selectedFeatures);
    QTableWidgetItem* itemFeatures = new QTableWidgetItem(features);

    QTableWidgetItem* itemCompany = new QTableWidgetItem(fnp.companyName);
    QTableWidgetItem* itemAddress = new QTableWidgetItem(fnp.address);
    QTableWidgetItem* itemDescription = new QTableWidgetItem(fnp.description);
    QTableWidgetItem* itemFilePath = new QTableWidgetItem(fnp.filePath);

    QTableWidgetItem* itemHidden = new QTableWidgetItem(QString::number(rowIndex));

    itemDate->setFlags(itemDate->flags() ^ Qt::ItemIsEditable);
    itemImage->setFlags(itemImage->flags() ^ Qt::ItemIsEditable);
    itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);
    itemFormworks->setFlags(itemFormworks->flags() ^ Qt::ItemIsEditable);
    itemFeatures->setFlags(itemFeatures->flags() ^ Qt::ItemIsEditable);
    itemCompany->setFlags(itemCompany->flags() ^ Qt::ItemIsEditable);
    itemAddress->setFlags(itemCompany->flags() ^ Qt::ItemIsEditable);
    itemDescription->setFlags(itemDescription->flags() ^ Qt::ItemIsEditable);
    itemFilePath->setFlags(itemDescription->flags() ^ Qt::ItemIsEditable);

    _tableResults->setItem(rowIndex, 0, itemHidden);
    _tableResults->setItem(rowIndex, 1, itemImage);
    _tableResults->setItem(rowIndex, 2, itemDate);
    _tableResults->setItem(rowIndex, 3, itemProjecNo);
    _tableResults->setItem(rowIndex, 4, itemName);
    _tableResults->setItem(rowIndex, 5, itemCompany);
    _tableResults->setItem(rowIndex, 6, itemAddress);
    _tableResults->setItem(rowIndex, 7, itemFormworks);
    _tableResults->setItem(rowIndex, 8, itemFeatures);
    _tableResults->setItem(rowIndex, 9, itemDescription);

    if (_cfg->ShowFilePathColumn()) _tableResults->setItem(rowIndex, 10, itemFilePath);
}

void SearchPage::fillSearchResultTable(int searchResultCount)
{
    if (searchResultCount == -1)
    {
        return;
    }
    _tableResults->setRowCount(searchResultCount);
    QStringList headers;
    headers << ""
            << tr("Preview")
            << tr("Photo date")
            << tr("Project No")
            << tr("Project name")
            << tr("Company")
            << tr("Address")
            << tr("Formwork systems")
            << tr("Features")
            << tr("Description");

    if (_cfg->ShowFilePathColumn()) headers << tr("File path");

    _tableResults->setHorizontalHeaderLabels(headers);
}

void SearchPage::finishSearch()
{
    emit searchFinished();
}
