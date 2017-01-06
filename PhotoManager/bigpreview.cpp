#include "bigpreview.h"
#include "ui_bigpreview.h"

BigPreview::BigPreview(const QString& fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BigPreview)
{
    ui->setupUi(this);

    QPixmap pm(fileName);

    ui->label->setPixmap(pm.scaled(800, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    setMouseTracking(true);
}

BigPreview::~BigPreview()
{
    delete ui;
}

void BigPreview::mouseMoveEvent(QMouseEvent *event)
{
    if (!ui->label->geometry().contains(event->pos()))
    {
        this->close();
    }
}
