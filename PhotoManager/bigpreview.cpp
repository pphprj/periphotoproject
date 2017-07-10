#include "bigpreview.h"
#include "ui_bigpreview.h"

BigPreview::BigPreview(const QString& fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BigPreview)
{
    ui->setupUi(this);

    _pm = new QPixmap(fileName);

    ui->label->setPixmap(_pm->scaled(800, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->label->installEventFilter(this);
}

BigPreview::~BigPreview()
{
    delete _pm;
    ui->label->clear();
    delete ui;
}

void BigPreview::mouseMoveEvent(QMouseEvent *event)
{
    if (!ui->label->geometry().contains(event->pos()))
    {
        this->close();
    }
}

bool BigPreview::eventFilter(QObject *watched, QEvent *event)
{
    if ((watched == ui->label) && (event->type() == QEvent::MouseButtonDblClick))
    {
        this->close();
        return true;
    }
    else
    {
        if (event->type() == QEvent::KeyRelease)
        {
            QKeyEvent *ke = static_cast<QKeyEvent *>(event);
            if (ke->key() == Qt::Key_Escape)
            {
                this->close();
                return true;
            }
        }
    }

    return false;
}
