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
    virtual QString GetUpdateRequest();

    int GetID();
    QString GetName();
    QString GetDesription();

    void SetName(const QString& name);
    void SetDescription(const QString& description);
protected:
    int _id;
    QString _name;
    QString _description;
};

#endif // TABLEABSTRACTELEM_H
