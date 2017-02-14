#include "interfacemanager.h"

#include <QCompleter>
#include <QLineEdit>

InterfaceManager::InterfaceManager(QObject *parent) : QObject(parent)
{

}

void InterfaceManager::NewItem(QTableWidget *table)
{
    QTableWidgetItem* item = new QTableWidgetItem(tr("New item"));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    table->insertRow(table->rowCount());
    table->setItem(table->rowCount() - 1, 0, item);
    table->setCurrentItem(item);
}

void InterfaceManager::ClearComboboxChecked(QComboBox *comboBox)
{
    QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(comboBox->model());
    for (int i = 0; i < standardModel->rowCount(); i++)
    {
        if (standardModel->item(i)->checkState() == Qt::Checked)
        {
            standardModel->item(i)->setCheckState(Qt::Unchecked);
        }
    }
    comboBox->setItemText(0, "");
}


void InterfaceManager::SetCompleter(QLineEdit *lineEdit, const QStringList& items)
{
    QCompleter* completer = new QCompleter(items);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    lineEdit->setCompleter(completer);
}


void InterfaceManager::EnableDateEditField(QDateEdit *dateEdit, bool isEnabled)
{
    if (dateEdit == nullptr) return;
    if (isEnabled)
    {
        dateEdit->setEnabled(true);
        dateEdit->setDateTime(QDateTime::currentDateTime());
    }
    else
    {
        dateEdit->setEnabled(false);
        dateEdit->setDateTime(QDateTime(QDate(1970, 1, 1)));
    }
}
