#include "aboutwindow.h"
#include "ui_aboutwindow.h"

#include <QIcon>
#include <QPixmap>


AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    QString version = "1.4.7";
    QString build = "build 2018-02-25 11:00";
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
