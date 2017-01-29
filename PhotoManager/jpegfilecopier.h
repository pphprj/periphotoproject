#ifndef JPEGFILECOPY_H
#define JPEGFILECOPY_H

#include <QString>
#include <QDateTime>

class JpegFileCopier
{
public:
    static bool copyFile(const QString& sourceFileName, const QString& destinationFileName, QDateTime& creationDate, int compression = 75);

private:
    JpegFileCopier();
};

#endif // JPEGFILECOPY_H
