#ifndef FILEINFOSTRUCT_H
#define FILEINFOSTRUCT_H

#include <QFileInfo>
#include <QDateTime>

struct FileInfoStruct
{
    QFileInfo fileInfo;
    QDateTime lastModified;
};

struct FileAndPreview
{
    QString filePath;
    QString previewPath;
    int photoId;
};

#endif // FILEINFOSTRUCT_H
