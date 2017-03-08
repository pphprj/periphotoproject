#ifndef SEARCHPAGEMODEL_H
#define SEARCHPAGEMODEL_H

#include <QStandardItemModel>

#include "fileinfostruct.h"

class SearchPageModel : public QStandardItemModel
{
    Q_OBJECT
public:
    SearchPageModel();
    virtual ~SearchPageModel();

    virtual void sort(int column, Qt::SortOrder order);
    void setSearchResults(const QVector<SearchResult>& searchResults);
private:
    void sortResultsByDate(Qt::SortOrder order);
    void sortResults(int column, Qt::SortOrder order);
private:
    QVector<SearchResult>& _searchResults;
};

#endif // SEARCHPAGEMODEL_H
