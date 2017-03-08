#include "searchpagemodel.h"

SearchPageModel::SearchPageModel()
    : QStandardItemModel()
{

}

SearchPageModel::~SearchPageModel()
{

}

void SearchPageModel::sort(int column, Qt::SortOrder order)
{
    sortResults(column, order);

}


void SearchPageModel::setSearchResults(const QVector<SearchResult> &searchResults)
{
    _searchResults = searchResults;
}

void SearchPageModel::sortResults(int column, Qt::SortOrder order)
{
    switch (column) {
    case 1:
        sortResultsByDate(order);
        break;
    default:
        break;
    }
}

bool dateAscending(const SearchResult& elem1, const SearchResult& elem2)
{
    QString s1 = elem1.photoDate.toString("yyyy-MM-dd");
    QString s2 = elem2.photoDate.toString("yyyy-MM-dd");
    int result = s1.compare(s2);
    return result < 0;
}

bool dateDescending(const SearchResult& elem1, const SearchResult& elem2)
{
    QString s1 = elem1.photoDate.toString("yyyy-MM-dd");
    QString s2 = elem2.photoDate.toString("yyyy-MM-dd");
    int result = s1.compare(s2);
    return result > 0;
}

void SearchPageModel::sortResultsByDate(Qt::SortOrder order)
{
    if (order == Qt::AscendingOrder)
    {
        std::stable_sort(_searchResults.begin(), _searchResults.end(), dateAscending);
    }
    else
    {
        std::stable_sort(_searchResults.begin(), _searchResults.end(), dateDescending);
    }
}
