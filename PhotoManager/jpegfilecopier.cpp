#include "jpegfilecopier.h"

#include <QPixmap>
#include <QFile>
#include <QTemporaryFile>
#include <QDebug>

#include <exiv2/exiv2.hpp>

JpegFileCopier::JpegFileCopier()
{
}

QDateTime JpegFileCopier::getCreationDate(const QString &sourceFileName)
{
    QDateTime result;

    Exiv2::Image::AutoPtr imageSource = Exiv2::ImageFactory::open(sourceFileName.toStdWString());
    if (imageSource.get() != nullptr)
    {
        imageSource->readMetadata();
        Exiv2::ExifData &ed = imageSource->exifData();

        if (!ed.empty())
        {
            Exiv2::Exifdatum& tag = ed["Exif.Image.DateTime"];
            std::string date = tag.toString();
            QString crd = QString::fromStdString(date);
            result = QDateTime::fromString(crd, "yyyy:MM:dd hh:mm:ss");
            qDebug() << "creation date " << crd;
            qDebug() << "creation date is valid " << result.isValid();
        }
    }

    return result;
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
         qDebug() << "open src jpeg file " << result;
         if (result)
         {
             imageSource->readMetadata();
             Exiv2::ExifData &ed = imageSource->exifData();
             qDebug() << "exif data is empty " << ed.empty();

             Exiv2::Image::AutoPtr imageDestination = Exiv2::ImageFactory::open(destinationFileName.toStdWString());
             result = (imageDestination.get() != nullptr);
             if (result)
             {
                 qDebug() << "open dest jpeg file " << result;
                 imageDestination->setExifData(ed);
                 imageDestination->writeMetadata();
             }
         }
    }

    return result;
}
