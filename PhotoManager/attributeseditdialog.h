#ifndef ATTRIBUTESEDITDIALOG_H
#define ATTRIBUTESEDITDIALOG_H

#include <QDialog>

namespace Ui {
class AttributesEditDialog;
}

class AttributesEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttributesEditDialog(QWidget *parent = 0);
    ~AttributesEditDialog();

private:
    Ui::AttributesEditDialog *ui;
};

#endif // ATTRIBUTESEDITDIALOG_H
