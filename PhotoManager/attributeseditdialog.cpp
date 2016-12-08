#include "attributeseditdialog.h"
#include "ui_attributeseditdialog.h"

AttributesEditDialog::AttributesEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttributesEditDialog)
{
    ui->setupUi(this);
}

AttributesEditDialog::~AttributesEditDialog()
{
    delete ui;
}
