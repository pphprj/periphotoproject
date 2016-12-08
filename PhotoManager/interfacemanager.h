#ifndef INTERFACELOADER_H
#define INTERFACELOADER_H

#include <QObject>
#include <QComboBox>
#include <QTableWidget>
#include <QStandardItem>

#include "tableabstractelemmanager.h"

class InterfaceManager : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceManager(QObject *parent = 0);

    template <typename T> static void FillCombobox(const QVector<T>& elems, const QComboBox* comboBox)
    {
        QComboBox* cb = const_cast<QComboBox*>(comboBox);

        cb->clear();

        QStandardItemModel* model = new QStandardItemModel(elems.length() + 2, 1);

        QStandardItem* itemShowSelecatble = new QStandardItem("");
        model->setItem(0, 0, itemShowSelecatble);


        QStandardItem* itemDisableAll = new QStandardItem(tr("Disable all"));

        itemDisableAll->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        itemDisableAll->setData(Qt::Unchecked, Qt::CheckStateRole);

        model->setItem(1, 0, itemDisableAll);

        for (int i = 2; i < elems.length() + 2; i++)
        {
            T elem = elems[i - 2];
            QStandardItem* item = new QStandardItem(elem.GetName());

            item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            item->setData(Qt::Unchecked, Qt::CheckStateRole);

            model->setItem(i, 0, item);
        }


        cb->setModel(model);
        cb->setCurrentIndex(0);
    }

    template <typename T> static void FillTableWidget(const QVector<T>& elems, const QTableWidget* table)
    {
        QTableWidget* tbWidget = const_cast<QTableWidget*>(table);
        tbWidget->setRowCount(elems.length());

        tbWidget->setColumnCount(2);
        for (int i = 0; i < elems.length(); i++)
        {
            T elem = elems[i];
            QTableWidgetItem* itemName = new QTableWidgetItem(elem.GetName());
            itemName->setFlags(itemName->flags() | Qt::ItemIsEditable);

            QString str = elem.GetDesription();
            QTableWidgetItem* itemDescription = new QTableWidgetItem(str);
            itemDescription->setFlags(itemDescription->flags() | Qt::ItemIsEditable);

            tbWidget->setItem(i, 0, itemName);
            tbWidget->setItem(i, 1, itemDescription);
        }
        tbWidget->resizeColumnsToContents();
    }

    template <typename T> static QVector<T> GetSelectedListItems(const QVector<T>& elems, const QAbstractItemModel* model)
    {
        QVector<T> selected;
        QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(model));
        int length = standardModel->rowCount();
        for (int i = 2; i < length; i++)
        {
            if (standardModel->item(i)->checkState() == Qt::Checked)
            {
                selected.push_back(elems[i - 2]);
            }
        }
        return selected;
    }

    template <typename T> static void ApplyChanges(QVector<T>& elems, const QTableWidget* table)
    {
        QTableWidget* tbWidget = const_cast<QTableWidget*>(table);
        for (int i = 0; i < tbWidget->rowCount(); i++)
        {
           QTableWidgetItem* name = tbWidget->item(i, 0);
           QTableWidgetItem* descr = tbWidget->item(i, 1);

           if (name == nullptr)
           {
               continue;
           }

           QString nameText = name->text();
           QString descrText = (descr != nullptr) ? descr->text() : "";

           if (i < elems.length())
           {
               elems[i].SetName(nameText);
               elems[i].SetDescription(descrText);
           }
           else
           {
               if (nameText != tr("New item"))
               {
                   elems.push_back(T(0, nameText, descrText));
               }
           }
        }
    }

    template <typename T> static void ItemChanged(QVector<T>& elems, QTableWidgetItem* item, QTableWidget* table)
    {
        if (item->text().isEmpty() && item->column() == 0)
        {
            if (item->row() < elems.length())
            {
                item->setText(elems[item->row()].GetName());
            }
            else
            {
                //if you no enter new name
                table->removeRow(item->row());
            }
        }
    }

    template <typename T> static void ShowSelection(const QVector<T>& selected, QComboBox* comboBox)
    {
        QString selectedList = TableAbstractElemManager::CreateNamesList(selected);
        comboBox->setItemText(0, selectedList);
    }

    static void NewItem(QTableWidget* table);
    static void ClearComboboxChecked(QComboBox* comboBox);

signals:

public slots:
};

#endif // INTERFACELOADER_H
