#ifndef IMAGEPRINTER_H
#define IMAGEPRINTER_H

#include <QObject>
#include <QPrinter>

class ImagePrinter : public QObject
{
    Q_OBJECT
public:
    explicit ImagePrinter(QObject *parent = 0);
    ~ImagePrinter();

    bool ShowDialog();
    bool PrintImage(const QString& file);

signals:

public slots:
private:
    QPrinter* _printer;
};

#endif // IMAGEPRINTER_H
