#ifndef TABLEABSTRACTELEMMANAGER_H
#define TABLEABSTRACTELEMMANAGER_H

#include <QString>

class TableAbstractElemManager
{
public:
    TableAbstractElemManager();

    template <typename T> static QString CreateIDsList(const QVector<T>& elems)
    {
        QString result;

        for(int i = 0; i < elems.length(); i++)
        {
            T elem = elems[i];
            result += QString::number(elem.GetID()) + ";";
        }

        result.remove(result.length() - 1, 1);

        return result;
    }

    template <typename T> static QString CreateNamesList(const QVector<T>& elems)
    {
        QString result;

        for(int i = 0; i < elems.length(); i++)
        {
            T elem = elems[i];
            result += elem.GetName() + ";";
        }

        result.remove(result.length() - 1, 1);

        return result;
    }

};

#endif // TABLEABSTRACTELEMMANAGER_H
