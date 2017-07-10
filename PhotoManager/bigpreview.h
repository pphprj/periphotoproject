#ifndef BIGPREVIEW_H
#define BIGPREVIEW_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class BigPreview;
}

class BigPreview : public QDialog
{
    Q_OBJECT

public:
    explicit BigPreview(const QString& fileName, QWidget *parent = 0);
    ~BigPreview();

private:
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QPixmap* _pm;
    Ui::BigPreview *ui;
};

#endif // BIGPREVIEW_H
