#ifndef JPEGFILECOPY_H
#define JPEGFILECOPY_H

#include <QString>

class JpegFileCopier
{
public:
    static bool copyFile(const QString& sourceFileName, const QString& destinationFileName, int compression = 75);

private:
    JpegFileCopier();
};

#endif // JPEGFILECOPY_H
