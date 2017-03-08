#ifndef FILEINFOSTRUCT_H
#define FILEINFOSTRUCT_H

#include <QFileInfo>
#include <QDateTime>
#include <QDebug>

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
    QString projectNo;
    QString companyName;
    QString description;
    QDate photoDate;

    friend QDebug operator <<(QDebug ds, const SearchResult& item)
    {
        ds << "////////";
        ds << "PhotoID=" << item.photoId;
        ds << "PhotoDate=" << item.photoDate.toString("yyyy-MM-dd");
        ds << "ProjectName=" << item.projectName;
        ds << "ProjectNo=" << item.projectNo;
        ds << "FilePath=" << item.filePath;
        ds << "PreviewPath=" << item.previewPath;
        ds << "Cats=" << item.categories;
        ds << "////////";
        return ds;
    }
};

#endif // FILEINFOSTRUCT_H
