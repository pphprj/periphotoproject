#ifndef RESULTCONTEXTMENU_H
#define RESULTCONTEXTMENU_H

#include <QObject>
#include <QTableView>
#include <QMenu>
#include <QAction>

class ResultContextMenu : public QObject
{
    Q_OBJECT
public:
    explicit ResultContextMenu(QPoint pos, QTableView* table, bool showRemove, QObject *parent = 0);
    ~ResultContextMenu();

    void Execute();

signals:
    void save(const QModelIndex& item);
    void print(const QModelIndex& item);
    void edit(const QModelIndex& item);
    void remove(const QModelIndex& remove);

private:
    QTableView* _table;
    QPoint _pos;
    QMenu* _contextMenu;
    QAction* _save;
    QAction* _print;
    QAction* _edit;
    QAction* _remove;
};

#endif // RESULTCONTEXTMENU_H
