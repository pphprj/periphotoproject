#include "imageprinter.h"

#include <QPrintDialog>
#include <QPainter>

ImagePrinter::ImagePrinter(QObject *parent) : QObject(parent)
{
    _printer = new QPrinter(QPrinter::HighResolution);
    _printer->setFullPage(true);
    _printer->setPaperSize(QPrinter::A4);
    _printer->setPageSize(QPrinter::A4);
}

ImagePrinter::~ImagePrinter()
{
    delete _printer;
}

bool ImagePrinter::ShowDialog()
{
    QPrintDialog *dlg = new QPrintDialog(_printer);
    bool result =(dlg->exec() == QDialog::Accepted);
    delete dlg;
    return result;
}

bool ImagePrinter::PrintImage(const QString &file)
{
    try
    {
        QPixmap pm(file);
        QPainter painter;
        painter.begin(_printer);
        _printer->newPage();
        QRect rect = painter.viewport();
        QSize size = pm.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(pm.rect());
        painter.drawPixmap(0, 0, pm);
        painter.end();
    }
    catch(...)
    {

    }
    return true;
}
