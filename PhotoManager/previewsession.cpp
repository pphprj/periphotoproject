#include "previewsession.h"
#include "previewworker.h"

#include <QPixmap>
#include <QThread>

PreviewSession::PreviewSession(QListWidget *list, int maxThreads, QObject *parent)
    : QObject(parent)
{
    _previewList = list;
    _maxThreads = maxThreads;
}

void PreviewSession::addFile(const QString &fileName)
{
    _files.push_back(fileName);
    addThread(fileName);
}

void PreviewSession::addFiles(const QStringList &list)
{
    _files = list;
}

void PreviewSession::addThread(const QString &fileName)
{
    QThread* thread = new QThread();
    PreviewWorker* worker = new PreviewWorker();
    worker->setFile(fileName);
    worker->moveToThread(thread);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(worker, SIGNAL(resultsReady(QPixmap, QString)), SLOT(finishedProcessPreview(QPixmap, QString)));
    thread->start();
}

void PreviewSession::finishedProcessPreview(QPixmap scaled, QString fileName)
{
    QMutexLocker lock(&_previewMutex);
    _previewList->addItem(new QListWidgetItem(QIcon(scaled), fileName));
    _previewList->sortItems();
}

