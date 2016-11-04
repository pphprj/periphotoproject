#ifndef PREVIEWWORKER_H
#define PREVIEWWORKER_H

#include <QObject>
#include <QPixmap>

class PreviewWorker : public QObject
{
    Q_OBJECT
public:
    explicit PreviewWorker(QObject *parent = 0);
    ~PreviewWorker();
    void setFile(const QString& file);
    QPixmap  getPixmap();
    QString getFileName();

signals:
    void resultsReady(QPixmap scaled, QString fileName);
    void finished();
    void error(QString err);

public slots:
    void process();

private:
    QString _fileName;
    QPixmap _pixmap;
};

#endif // PREVIEWWORKER_H
