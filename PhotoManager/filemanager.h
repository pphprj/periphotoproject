#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QDateTime>

class Filemanager
{
public:
    Filemanager(const QString& root);

    void CreateRootDirectory();
    bool CreateProjectDirectory(const QString& projectNo, const QDateTime& time);

    //return destination file name
    QString AddFileToDirectory(const QString& file);
    QStringList AddFilesToDirectory(const QStringList& files);

    bool CheckDirectory(const QString& path);

private:
    bool CreateDirectory(const QString& path);

private:
    QString _rootDirectory;
    QString _projectDirectory;
};

#endif // FILEMANAGER_H
