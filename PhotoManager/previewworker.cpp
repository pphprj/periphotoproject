#include "previewworker.h"

#include <QFileInfo>
#include <QImageReader>
#include <QRect>

PreviewWorker::PreviewWorker(QObject *parent) : QObject(parent)
{
}

PreviewWorker::~PreviewWorker()
{

}

void PreviewWorker::setFile(const QString &file)
{
    _fileName = file;
}

QPixmap PreviewWorker::getPixmap()
{
    return _pixmap;
}

QString PreviewWorker::getFileName()
{
    return _fileName;
}

void PreviewWorker::process()
{
    QPixmap pm;
    QFileInfo info(_fileName);
    if (info.suffix() == "pdf")
    {
        pm = QPixmap(":/pdfs-512.png");
    }
    else
    {
       // QImageReader imr(_fileName);
        //imr.setClipRect(QRect(0, 0, 400, 300));
       // imr.setScaledSize(QSize(400, 300));
        pm.load(_fileName);
        //QImage img = imr.read();
        //pm = QPixmap::fromImage(img);
    }
    _pixmap = pm.scaled(400, 300, Qt::KeepAspectRatio).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //_pixmap = pm.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    emit resultsReady(_pixmap, _fileName);
    emit finished();
}
