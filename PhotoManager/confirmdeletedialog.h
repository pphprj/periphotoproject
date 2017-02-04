#ifndef CONFIRMDELETEDIALOG_H
#define CONFIRMDELETEDIALOG_H

#include <QDialog>
#include "passwordmanager.h"

namespace Ui {
class ConfirmDeleteDialog;
}

class ConfirmDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDeleteDialog(const QString& password, QWidget *parent = 0);
    ~ConfirmDeleteDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ConfirmDeleteDialog *ui;
    PasswordManager* _pwdManager;
};

#endif // CONFIRMDELETEDIALOG_H
