#ifndef FILEINFOSTRUCT_H
#define FILEINFOSTRUCT_H

#include <QFileInfo>
#include <QDateTime>

struct FileInfoStruct
{
    QFileInfo fileInfo;
    QDateTime lastModified;
    bool duplicated;
};

struct SearchResult
{
    QString filePath;
    QString previewPath;

    int photoId;
    QString categories;
    QString projectName;
    QDate photoDate;
};

#endif // FILEINFOSTRUCT_H
