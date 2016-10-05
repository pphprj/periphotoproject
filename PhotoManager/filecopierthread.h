#ifndef FILECOPIERTHREAD_H
#define FILECOPIERTHREAD_H

#include <QThread>
#include <QString>
#include <QFileInfo>
#include <QVector>

class FileManager;

class FilecopierThread : public QThread
{
    Q_OBJECT

public:
    FilecopierThread(FileManager* manager);
    void setFileList(QStringList files, int compression);
    void run();

    QVector<QFileInfo>& getCopiedFiles();

signals:
    void progress(int value);
    //void finishedCopy(QVector<QFileInfo> copiedFiles);
    void finished();

private:
    QStringList _files;
    int _compression;
    FileManager* _fileManager;
    QVector<QFileInfo> _copiedFiles;
};

#endif // FILECOPIERTHREAD_H
