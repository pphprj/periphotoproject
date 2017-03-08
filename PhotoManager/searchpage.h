#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QTableView>

#include "page.h"
#include "photosearcher.h"
#include "searchpagemodel.h"


class SearchPage : public Page
{
    Q_OBJECT
public:
    explicit SearchPage(DatabaseManager* databaseManager, FileManager* fileManager, Configurator* cfg, QObject *parent = 0);
    ~SearchPage();

    virtual void CreateLoader();
    virtual void ClearInterface();

    void InitSearchPageInterface(QDateEdit* beginInterval, QDateEdit* endInterval, QTableView* tableResults,
                                 const QVector<QCheckBox*>& filterCategories, QCheckBox* filterByDate, QDateEdit* filterDate);

    void SearchPhotos();
    void SavePhotos();
    void PrintPhotos();
    void DeletePhotos();

    void RefreshProjects();

    void OrderDateOnly(const QDate& date);
    void OrderByDate();

    void ShowItemPreview(int index);

public slots:
    void showContextMenu(QPoint pos);

    void saveSelected(const QModelIndex& item);
    void printSelected(const QModelIndex& item);
    void removeSelected(const QModelIndex& item);
    void editSelected(const QModelIndex& item);

    void sectionClickedSlot(int column);
    void checkBoxClickedSlot();

private:
    void RemoveItem(const QModelIndex& item);

private:
    QDateEdit* _beginInterval;
    QDateEdit* _endInterval;
    QTableView* _tableResults;
    QVector<QCheckBox*> _filterCategories;
    QCheckBox* _filterByDate;
    QDateEdit* _filterDate;

    PhotoSearcher* _searcher;
    QVector<SearchResult> _searchResult;
};

#endif // SEARCHPAGE_H
