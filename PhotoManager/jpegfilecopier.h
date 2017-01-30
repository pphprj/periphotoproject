#ifndef JPEGFILECOPY_H
#define JPEGFILECOPY_H

#include <QString>
#include <QDateTime>

class JpegFileCopier
{
public:
    static bool copyFile(const QString& sourceFileName, const QString& destinationFileName, int compression = 75);
    static QDateTime getCreationDate(const QString& sourceFileName);

private:
    JpegFileCopier();
};

#endif // JPEGFILECOPY_H
