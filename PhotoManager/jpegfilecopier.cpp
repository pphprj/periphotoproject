#include "jpegfilecopier.h"

#include <QPixmap>
#include <QFile>
#include <QTemporaryFile>
#include <QDebug>

#include <exiv2/exiv2.hpp>

JpegFileCopier::JpegFileCopier()
{

}


bool JpegFileCopier::copyFile(const QString &sourceFileName, const QString &destinationFileName, int compression)
{
    bool result = false;
    QPixmap sourceImage(sourceFileName);
    QTemporaryFile tmpFile;
    tmpFile.setAutoRemove(true);
    if (tmpFile.open())
    {
       result = sourceImage.save(&tmpFile, "JPEG", compression);
       qDebug() << "compression result " << result;
       if (!result)
       {
           return result;
       }
    }

    if (QFile::exists(destinationFileName))
    {
        QFile::remove(destinationFileName);
    }

    result = QFile::copy(tmpFile.fileName(), destinationFileName);

    if (result)
    {
         Exiv2::Image::AutoPtr imageSource = Exiv2::ImageFactory::open(sourceFileName.toStdWString());
         result = imageSource.get() != nullptr;
         if (result)
         {
             imageSource->readMetadata();
             Exiv2::ExifData &ed = imageSource->exifData();
             Exiv2::Image::AutoPtr imageDestination = Exiv2::ImageFactory::open(destinationFileName.toStdWString());
             result = (imageDestination.get() != nullptr);
             if (result)
             {
                 imageDestination->setExifData(ed);
                 imageDestination->writeMetadata();
             }
         }
    }

    return result;
}
