#include "filemanager.h"

#include <QDir>
#include <QFile>
#include <QPixmap>
#include <QTemporaryFile>
#include <QDebug>
#include <QIcon>


FileManager::FileManager(const QString& root)
{
    _rootDirectory = root;
}

bool FileManager::CheckDirectory(const QString &path)
{
    QDir directory;
    return directory.exists(path);
}

bool FileManager::CreateDirectory(const QString &path)
{
    QDir directory;
    return directory.mkpath(path);
}

void FileManager::CreateRootDirectory()
{
    if (!CheckDirectory(_rootDirectory))
        CreateDirectory(_rootDirectory);
}

bool FileManager::CreateProjectDirectory(const QString &projectNo, const QString& projectName)
{
    _projectDirectory = _rootDirectory + "\\" + projectNo;

    if (!projectName.isEmpty())
        _projectDirectory += " " + projectName;

    if (!CheckDirectory(_projectDirectory))
        return CreateDirectory(_projectDirectory);
    return true;
}

bool FileManager::CreateFilesDirectory(const QDate &filesDate)
{
    QString filesDirectory = _projectDirectory + "\\" + filesDate.toString("yyyy-MM-dd") + "\\";
    if (!CheckDirectory(filesDirectory))
        return CreateDirectory(filesDirectory);
    return true;
}

bool FileManager::CreatePreviewDirectory(const QDate &filesDate)
{
    _previewDirectory = _projectDirectory + "\\" + filesDate.toString("yyyy-MM-dd") + "\\" + "preview" + "\\";
    if (!CheckDirectory(_previewDirectory))
        return CreateDirectory(_previewDirectory);
    return true;
}

FileInfoStruct FileManager::AddFileToDirectory(const QString &file, int compressionRate)
{
    QFileInfo sourceFile(file);
    CreateFilesDirectory(sourceFile.lastModified().date());

    QPixmap sourceImage(file);
    //QPixmap destinationImage = sourceImage.scaled(1024, 768, Qt::AspectRatioMode::KeepAspectRatioByExpanding, Qt::TransformationMode::SmoothTransformation);
    //destinationImage.save(destinationFileName, "JPEG", compressionRate);
    QTemporaryFile tmpFile;
    tmpFile.setAutoRemove(true);
    if (tmpFile.open())
    {
       bool res = sourceImage.save(&tmpFile, "JPEG", compressionRate);
       qDebug() << "compression result " << res;
    }

    QString destination = _projectDirectory + "\\" + sourceFile.lastModified().date().toString("yyyy-MM-dd") + "\\";
    QString destinationFileName = destination + sourceFile.fileName();

    if (QFile::exists(destinationFileName))
    {
        QFile::remove(destinationFileName);
    }

    if (QFile::copy(tmpFile.fileName(), destinationFileName))
    {
        qDebug() << "copy was successfull";
        FileInfoStruct fileInfo;
        fileInfo.fileInfo.setFile(destinationFileName);
        fileInfo.lastModified = sourceFile.lastModified();
        return fileInfo;
    }
    return FileInfoStruct();
}

QVector<FileInfoStruct> FileManager::AddFilesToDirectory(const QStringList &files, int compressionRate)
{
    QVector<FileInfoStruct> result;
    for (int i = 0; i < files.length(); i++)
    {
        result.push_back(AddFileToDirectory(files.at(i), compressionRate));
    }
    return result;
}

QFileInfo FileManager::AddPreviewToDirectory(const QIcon &icon, const QString &file)
{
    QFileInfo sourceFile(file);
    CreatePreviewDirectory(sourceFile.lastModified().date());

    QString previewFileName = _previewDirectory + "\\" + sourceFile.baseName() + ".png";

    icon.pixmap(100, 100).save(previewFileName);

    return QFileInfo(previewFileName);
}
