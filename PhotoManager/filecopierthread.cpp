#include "filecopierthread.h"
#include "filemanager.h"

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
    _copiedFiles.clear();
    int delta = 100 / _files.length();
    for (int i = 0; i < _files.length(); i++)
    {
        _copiedFiles.push_back(_fileManager->AddFileToDirectory(_files.at(i), _compression));
        emit progress((i + 1) * delta);
    }
    emit progress(100);
    emit finished();
}

QVector<QFileInfo>& FilecopierThread::getCopiedFiles()
{
    return _copiedFiles;
}


