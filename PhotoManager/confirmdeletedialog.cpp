#include "confirmdeletedialog.h"
#include "ui_confirmdeletedialog.h"

ConfirmDeleteDialog::ConfirmDeleteDialog(const QString& password, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDeleteDialog)
{
    ui->setupUi(this);
    _pwdManager = new PasswordManager(password);
}

ConfirmDeleteDialog::~ConfirmDeleteDialog()
{
    delete _pwdManager;
    delete ui;
}

void ConfirmDeleteDialog::on_buttonBox_accepted()
{
    if (_pwdManager->CheckPassword(ui->lineEditPassword->text()))
    {
        this->setResult(QDialog::Accepted);
    }
    else
    {
        this->setResult(QDialog::Rejected);
    }
}
