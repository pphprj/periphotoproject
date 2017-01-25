#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QDateTime>
#include <QVector>

#include "fileinfostruct.h"

class FileManager
{
public:
    FileManager(const QString& root);

    void CreateRootDirectory();
    bool CreateProjectDirectory(const QString& projectNo, const QString& projectName);
    bool CreateFilesDirectory(const QDate& filesDate);
    bool CreatePreviewDirectory(const QDate& filesDate);

    //return destination file name
    FileInfoStruct AddFileToDirectory(const QString& file, int compressionRate);
    QVector<FileInfoStruct> AddFilesToDirectory(const QStringList& files, int compressionRate);
    QFileInfo AddPreviewToDirectory(const QIcon& icon, const QString& file);

    bool DeleteFilesFromDirectory(const QStringList& files);
    bool DeleteFileFromDirectory(const QString& file);

    static bool CheckDirectory(const QString& path);

private:
    bool CreateDirectory(const QString& path);

private:
    QString _rootDirectory;
    QString _projectDirectory;
    QString _previewDirectory;
};

#endif // FILEMANAGER_H
