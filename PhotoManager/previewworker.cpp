#include "previewworker.h"

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
    QPixmap pm(_fileName);
    _pixmap = pm.scaled(400, 300, Qt::KeepAspectRatio).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    emit resultsReady(_pixmap, _fileName);
    emit finished();
}
