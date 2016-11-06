#ifndef PREVIEWSESSION_H
#define PREVIEWSESSION_H

#include <QObject>
#include <QMutex>
#include <QListWidget>


class PreviewSession : public QObject
{
    Q_OBJECT
public:
    explicit PreviewSession(QListWidget* list, int maxThreads = 20, QObject *parent = 0);
    void addFile(const QString& fileName);
    void addFiles(const QStringList& list);

signals:

public slots:
    void finishedProcessPreview(QPixmap scaled, QString fileName);

private:
    void addThread(const QString& fileName);

private:
    int _maxThreads;
    QStringList _files;
    QMutex _previewMutex;
    QListWidget* _previewList;
};

#endif // PREVIEWSESSION_H
