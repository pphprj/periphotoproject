#ifndef TABLEABSTRACTELEM_H
#define TABLEABSTRACTELEM_H

#include <QString>

class TableAbstractElem
{
public:
    TableAbstractElem();
    TableAbstractElem(int id, const QString& name, const QString& descr);
    ~TableAbstractElem();

    virtual QString GetInsertRequest();

    int GetID();
    QString GetName();
    QString GetDesription();
protected:
    int _id;
    QString _name;
    QString _description;
};

#endif // TABLEABSTRACTELEM_H
