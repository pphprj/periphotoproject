#include "interfacemanager.h"

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
