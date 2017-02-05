#include "filemanager.h"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QIcon>

#include "jpegfilecopier.h"

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
    FileInfoStruct fileInfo;

    QFileInfo sourceFile(file);
    QDateTime creationDate;
    if (sourceFile.suffix() != "pdf")
    {
        creationDate = JpegFileCopier::getCreationDate(file);
    }
    creationDate = (creationDate.isNull()) ? sourceFile.lastModified() : creationDate;
    CreateFilesDirectory(creationDate.date());

    QString destination = _projectDirectory + "\\" + sourceFile.lastModified().date().toString("yyyy-MM-dd") + "\\";
    QString destinationFileName = destination + sourceFile.fileName();

    fileInfo.duplicated = QFile::exists(destinationFileName);

    bool copyResult = false;
    if (sourceFile.suffix() == "pdf")
    {
        copyResult = QFile::copy(file, destinationFileName);
    }
    else
    {
        copyResult = JpegFileCopier::copyFile(file, destinationFileName, compressionRate);
    }

    if (copyResult)
    {
        qDebug() << "copy was successfull";
        fileInfo.fileInfo.setFile(destinationFileName);
        fileInfo.lastModified = creationDate;
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

bool FileManager::DeleteFilesFromDirectory(const QStringList &files)
{
    bool result = true;
    for (int i = 0; i < files.length(); i++)
    {
        result |= DeleteFileFromDirectory(files.at(i));
    }
    return result;
}

bool FileManager::DeleteFileFromDirectory(const QString &file)
{
    return QFile::remove(file);
}
