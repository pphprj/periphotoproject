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

bool Filemanager::CreateProjectDirectory(const QString &projectNo, const QDate &projectDate)
{
    _projectDirectory = _rootDirectory + "\\" + projectNo + "\\" + projectDate.toString("yyyy-MM-dd") + "\\";
    if (!CheckDirectory(_projectDirectory))
        CreateDirectory(_projectDirectory);
    return true;
}

QString Filemanager::AddFileToDirectory(const QString &file)
{
    int delimPos = file.lastIndexOf("/");
    QStringRef sourceFileName(&file, delimPos + 1, file.length() - delimPos - 1);
    QString destinationFileName = _projectDirectory + sourceFileName.toString();
    if (QFile::copy(file, destinationFileName))
    {
        return destinationFileName;
    }
    return "";
}

QStringList Filemanager::AddFilesToDirectory(const QStringList &files)
{
    QStringList result;
    for (int i = 0; i < files.length(); i++)
    {
        result.push_back(AddFileToDirectory(files.at(i)));
    }
    return result;
}
