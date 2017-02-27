#ifndef RESULTCONTEXTMENU_H
#define RESULTCONTEXTMENU_H

#include <QObject>
#include <QTableWidget>
#include <QMenu>
#include <QAction>

class ResultContextMenu : public QObject
{
    Q_OBJECT
public:
    explicit ResultContextMenu(QPoint pos, QTableWidget* table, bool showRemove, QObject *parent = 0);
    ~ResultContextMenu();

    void Execute();

signals:
    void save(QTableWidgetItem* item);
    void print(QTableWidgetItem* item);
    void edit(QTableWidgetItem* item);
    void remove(QTableWidgetItem* remove);

private:
    QTableWidget* _table;
    QPoint _pos;
    QMenu* _contextMenu;
    QAction* _save;
    QAction* _print;
    QAction* _edit;
    QAction* _remove;
};

#endif // RESULTCONTEXTMENU_H
