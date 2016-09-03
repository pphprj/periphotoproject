#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QFileInfo>

class Filemanager
{
public:
    Filemanager(const QString& root);

    void CreateRootDirectory();
    bool CreateProjectDirectory(const QString& projectNo);
    bool CreateFilesDirectory(const QDate& filesDate);

    //return destination file name
    QFileInfo AddFileToDirectory(const QString& file);
    QVector<QFileInfo> AddFilesToDirectory(const QStringList& files);

    bool CheckDirectory(const QString& path);

private:
    bool CreateDirectory(const QString& path);

private:
    QString _rootDirectory;
    QString _projectDirectory;
};

#endif // FILEMANAGER_H
