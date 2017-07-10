#include "resultcontextmenu.h"

ResultContextMenu::ResultContextMenu(QPoint pos, QTableWidget *table, bool showRemove, QObject *parent) :
    QObject(parent)
{
    _table = table;
    _pos = pos;

    _contextMenu = new QMenu();
    _save = _contextMenu->addAction(tr("Save"));
    _print = _contextMenu->addAction(tr("Print"));
    _edit = _contextMenu->addAction(tr("Edit"));
    _remove = nullptr;
    if (showRemove)
    {
        _remove = _contextMenu->addAction(tr("Remove"));
    }
    _show = _contextMenu->addAction(tr("Show in folder"));

    connect(this, SIGNAL(save(QTableWidgetItem*)), parent, SLOT(saveSelected(QTableWidgetItem*)));
    connect(this, SIGNAL(print(QTableWidgetItem*)), parent, SLOT(printSelected(QTableWidgetItem*)));
    connect(this, SIGNAL(edit(QTableWidgetItem*)), parent, SLOT(editSelected(QTableWidgetItem*)));
    connect(this, SIGNAL(remove(QTableWidgetItem*)), parent, SLOT(removeSelected(QTableWidgetItem*)));
    connect(this, SIGNAL(show(QTableWidgetItem*)), parent, SLOT(showSelected(QTableWidgetItem*)));
}


ResultContextMenu::~ResultContextMenu()
{
    delete _contextMenu;
}

void ResultContextMenu::Execute()
{
    QTableWidgetItem* item = _table->itemAt(_pos);
    if (item == nullptr) return;

    QAction* selectedAction = _contextMenu->exec(_table->mapToGlobal(_pos));

    if (selectedAction)
    {
        if (selectedAction == _save)
        {
            emit save(item);
        }

        if (selectedAction == _print)
        {
            emit print(item);
        }

        if (selectedAction == _edit)
        {
            emit edit(item);
        }

        if (selectedAction == _remove)
        {
            emit remove(item);
        }

        if (selectedAction == _show)
        {
            emit show(item);
        }
    }
}
