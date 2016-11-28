#ifndef FILEINFOSTRUCT_H
#define FILEINFOSTRUCT_H

#include <QFileInfo>
#include <QDateTime>

struct FileInfoStruct
{
    QFileInfo fileInfo;
    QDateTime lastModified;
};

#endif // FILEINFOSTRUCT_H
