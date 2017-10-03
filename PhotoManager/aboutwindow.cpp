#include "aboutwindow.h"
#include "ui_aboutwindow.h"

#include <QIcon>
#include <QPixmap>


AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    QString version = "1.4.5";
    QString build = "build 2017-10-03 14:05";
    ui->labelVersion->setText(version + " " + build);

    ui->labelIcon->setPixmap(QPixmap(":/periphotoproject.ico"));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_buttonBox_clicked(QAbstractButton *button)
{
    this->close();
}
