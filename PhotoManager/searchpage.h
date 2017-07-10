#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QTableWidget>

#include "page.h"
#include "photosearcher.h"


class SearchPage : public Page
{
    Q_OBJECT
public:
    explicit SearchPage(DatabaseManager* databaseManager, FileManager* fileManager, Configurator* cfg, QObject *parent = 0);
    ~SearchPage();

    virtual void CreateLoader();
    virtual void ClearInterface();

    void InitSearchPageInterface(QDateEdit* beginInterval, QDateEdit* endInterval, QTableWidget* tableResults,
                                 const QVector<QCheckBox*>& filterCategories, QCheckBox* filterByDate, QDateEdit* filterDate);

    void SearchPhotos();
    void SavePhotos();
    void PrintPhotos();
    void DeletePhotos();

    void RefreshProjects();

    void OrderDateOnly(const QDate& date);
    void OrderByDate();

    void ShowItemPreview(QTableWidgetItem* item);

    int GetSearchResultIndexByItem(QTableWidgetItem* item);
    QTableWidgetItem* GetItemByIndex(int index);

public slots:
    void showContextMenu(QPoint pos);

    void saveSelected(QTableWidgetItem* item);
    void printSelected(QTableWidgetItem* item);
    void removeSelected(QTableWidgetItem* item);
    void editSelected(QTableWidgetItem* item);
    void showSelected(QTableWidgetItem* item);

    void sectionClickedSlot(int column);
    void checkBoxClickedSlot();
private:
    void RemoveItem(QTableWidgetItem* item);

private:
    QDateEdit* _beginInterval;
    QDateEdit* _endInterval;
    QTableWidget* _tableResults;
    QVector<QCheckBox*> _filterCategories;
    QCheckBox* _filterByDate;
    QDateEdit* _filterDate;

    PhotoSearcher* _searcher;
    QVector<SearchResult> _searchResult;
};

#endif // SEARCHPAGE_H
