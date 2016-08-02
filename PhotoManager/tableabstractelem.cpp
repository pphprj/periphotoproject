#include "tableabstractelem.h"

TableAbstractElem::TableAbstractElem()
{
    _id = 0;
    _name = "";
    _description = "";
}

TableAbstractElem::TableAbstractElem(int id, const QString& name, const QString& descr)
{
    _id = id;
    _name = name;
    _description = descr;
}

TableAbstractElem::~TableAbstractElem()
{

}

int TableAbstractElem::GetID()
{
    return _id;
}

QString TableAbstractElem::GetName()
{
    return _name;
}

QString TableAbstractElem::GetDesription()
{
    return _description;
}

QString TableAbstractElem::GetInsertRequest()
{
    return "";
}
