#include "resultcontextmenu.h"

#include <QStandardItemModel>

ResultContextMenu::ResultContextMenu(QPoint pos, QTableView *table, bool showRemove, QObject *parent) :
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

    connect(this, SIGNAL(save(const QModelIndex&)), parent, SLOT(saveSelected(const QModelIndex&)));
    connect(this, SIGNAL(print(const QModelIndex&)), parent, SLOT(printSelected(const QModelIndex&)));
    connect(this, SIGNAL(edit(const QModelIndex&)), parent, SLOT(editSelected(const QModelIndex&)));
    connect(this, SIGNAL(remove(const QModelIndex&)), parent, SLOT(removeSelected(const QModelIndex&)));
}


ResultContextMenu::~ResultContextMenu()
{
    delete _contextMenu;
}

void ResultContextMenu::Execute()
{
    //QStandardItemModel* model = qobject_cast<QStandardItemModel*>(_table->model());
    QModelIndex item = _table->indexAt(_pos);
    if (!item.isValid()) return;

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
    }
}
