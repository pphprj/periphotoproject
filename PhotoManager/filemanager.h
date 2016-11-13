#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QFileInfo>

class FileManager
{
public:
    FileManager(const QString& root);

    void CreateRootDirectory();
    bool CreateProjectDirectory(const QString& projectNo, const QString& projectName);
    bool CreateFilesDirectory(const QDate& filesDate);
    bool CreatePreviewDirectory(const QDate& filesDate);

    //return destination file name
    QFileInfo AddFileToDirectory(const QString& file, int compressionRate);
    QVector<QFileInfo> AddFilesToDirectory(const QStringList& files, int compressionRate);
    QFileInfo AddPreviewToDirectory(const QIcon& icon, const QString& file);

    bool CheckDirectory(const QString& path);

private:
    bool CreateDirectory(const QString& path);

private:
    QString _rootDirectory;
    QString _projectDirectory;
    QString _previewDirectory;
};

#endif // FILEMANAGER_H
