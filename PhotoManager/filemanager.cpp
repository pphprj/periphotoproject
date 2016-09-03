#include "filemanager.h"

#include <QDir>
#include <QFile>

Filemanager::Filemanager(const QString& root)
{
    _rootDirectory = root;
}

bool Filemanager::CheckDirectory(const QString &path)
{
    QDir directory;
    return directory.exists(path);
}

bool Filemanager::CreateDirectory(const QString &path)
{
    QDir directory;
    return directory.mkpath(path);
}

void Filemanager::CreateRootDirectory()
{
    if (!CheckDirectory(_rootDirectory))
        CreateDirectory(_rootDirectory);
}

bool Filemanager::CreateProjectDirectory(const QString &projectNo)
{
    _projectDirectory = _rootDirectory + "\\" + projectNo;
    if (!CheckDirectory(_projectDirectory))
        return CreateDirectory(_projectDirectory);
    return true;
}

bool Filemanager::CreateFilesDirectory(const QDate &filesDate)
{
    QString filesDirectory = _projectDirectory + "\\" + filesDate.toString("yyyy-MM-dd") + "\\";
    if (!CheckDirectory(filesDirectory))
        return CreateDirectory(filesDirectory);
    return true;
}

QFileInfo Filemanager::AddFileToDirectory(const QString &file)
{
    //int delimPos = file.lastIndexOf("/");
    //QStringRef sourceFileName(&file, delimPos + 1, file.length() - delimPos - 1);
    QFileInfo sourceFile(file);
    CreateFilesDirectory(sourceFile.lastModified().date());
    QString destination = _projectDirectory + "\\" + sourceFile.lastModified().date().toString("yyyy-MM-dd") + "\\";
    QString destinationFileName = destination + sourceFile.fileName();
    if (QFile::copy(file, destinationFileName))
    {
        return QFileInfo(destinationFileName);
    }
    return QFileInfo();
}

QVector<QFileInfo> Filemanager::AddFilesToDirectory(const QStringList &files)
{
    QVector<QFileInfo> result;
    for (int i = 0; i < files.length(); i++)
    {
        result.push_back(AddFileToDirectory(files.at(i)));
    }
    return result;
}
