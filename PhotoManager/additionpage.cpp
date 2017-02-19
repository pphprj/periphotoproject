#include "additionpage.h"

#include <QtGui>
#include <QMessageBox>

AdditionPage::AdditionPage(DatabaseManager *databaseManager,
                           FileManager *fileManager,
                           Configurator *cfg,
                           QObject *parent) :
    Page(databaseManager, fileManager, cfg, parent)
{
    _copierThread = new FilecopierThread(_fileManager);
    QObject::connect(_copierThread, SIGNAL(progress(int)), this, SLOT(setProgressBarValue(int)));
    QObject::connect(_copierThread, SIGNAL(finished()), this, SLOT(finishedCopy()));

    _previewSession = nullptr;
}

AdditionPage::~AdditionPage()
{
    delete _copierThread;
    if (_previewSession != nullptr)
    {
        delete _previewSession;
    }
}

void AdditionPage::CreateLoader()
{
    _loader = new PhotoLoader(_dbm);
}

void AdditionPage::InitAdditionPageInterface(QListWidget* photoList, QProgressBar* progress)
{
    _photoList = photoList;
    _previewSession = new PreviewSession(_photoList);
    _progress = progress;
}


void AdditionPage::AddFileToPreview(const QString &fileName)
{
    if (!_files.contains(fileName, Qt::CaseInsensitive))
    {
        _files.push_back(fileName);

        _previewSession->addFile(fileName);
    }
}

void AdditionPage::DeleteFileFromPreview(int num)
{
    if ((num < 0) || (num > _files.length()))
    {
        return;
    }

    delete _photoList->item(num);
    _files.removeAt(num);
}


void AdditionPage::AddFilesToDatabase()
{
    QString projectNo = _projectNo->text();
    if (projectNo.isEmpty())
    {
        QMessageBox::critical(nullptr, tr("Error!"), tr("Please, input Project No!"));
        return;
    }

    if (_selectedSystems.isEmpty())
    {
        QMessageBox::critical(nullptr, tr("Error!"), tr("Please, select formworks!"));
        return;
    }

    if (_selectedFeatures.isEmpty())
    {
        QMessageBox::critical(nullptr, tr("Error!"), tr("Please, select features!"));
        return;
    }

    QString selectedCategories = GetSelectedCategories();
    if (selectedCategories.isEmpty())
    {
        QMessageBox::critical(nullptr, tr("Error!"), tr("Please, select category!"));
        return;
    }

    if (_files.isEmpty())
    {
        QMessageBox::critical(nullptr, tr("Error!"), tr("Please, select photos!"));
        return;
    }

    QString projectName = _projectName->text();
    ProjectName pn = _loader->GetProjectNameByNo(projectNo);
    if (!pn.GetName().isEmpty())
    {
        projectName = (pn.GetName() == projectName) ? projectName : pn.GetName();
    }

    _fileManager->CreateProjectDirectory(projectNo, projectName);
    _copierThread->setFileList(_files, _cfg->GetCompressionRate());
    _copierThread->start();
}

void AdditionPage::ClearInterface()
{
    _photoList->clear();
    _progress->setValue(0);
    _files.clear();
}

void AdditionPage::setProgressBarValue(int value)
{
    _progress->setValue(value);
}

void AdditionPage::finishedCopy()
{
    qDebug() << "finishedCopy start";
    QVector<FileInfoStruct> files = _copierThread->getCopiedFiles();


    QVector<QFileInfo> previews;
    for (int i = 0; i < _files.count(); i++)
    {
       previews.push_back(_fileManager->AddPreviewToDirectory(_photoList->item(i)->icon(), _files.at(i)));
    }


    QString projectNo = _projectNo->text();
    QString projectName = _projectName->text();
    ProjectName pn = _loader->GetProjectNameByNo(projectNo);
    if (!pn.GetName().isEmpty())
    {
        projectName = (pn.GetName() == projectName) ? projectName : pn.GetName();
    }
    QDate projectDate = _projectDate->date();
    bool result = _loader->InsertToDatabase(projectNo, projectName, projectDate,
                                            _selectedSystems, _selectedFeatures, GetSelectedCategories(),
                                            files,
                                            previews);

    if (result)
    {
        _loader->RefreshProjectNames();
        //TODO refresh searcher
        //_searcher->RefreshProjectNames();
        QMessageBox::information(nullptr, tr("Successfully"), tr("Photos were added to DB"));
       // _photoList->clear();
        ClearInterface();
    }
    qDebug() << "finishedCopy end" << result;
}
