#include "filecopierthread.h"
#include "filemanager.h"

#include <QDebug>

FilecopierThread::FilecopierThread(FileManager* manager)
{
    _fileManager = manager;
}

void FilecopierThread::setFileList(QStringList files, int compression)
{
    _files = files;
    _compression = compression;
}

void FilecopierThread::run()
{
    qDebug() << "Thread copy start";
    _copiedFiles.clear();
    int delta = 100 / _files.length();
    for (int i = 0; i < _files.length(); i++)
    {
        _copiedFiles.push_back(_fileManager->AddFileToDirectory(_files.at(i), _compression));
        qDebug() << "File # " << i + 1;
        emit progress((i + 1) * delta);
    }
    qDebug() << "Circle finished";
    emit progress(100);
    qDebug() << "Progress 100";
    emit finished();
    qDebug() << "Thread copy finished";
}

QVector<QFileInfo>& FilecopierThread::getCopiedFiles()
{
    return _copiedFiles;
}


