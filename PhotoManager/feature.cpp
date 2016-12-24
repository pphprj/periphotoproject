#include "feature.h"

Feature::Feature() :
    TableAbstractElem()
{

}

Feature::Feature(int id, const QString& name, const QString& descr) :
    TableAbstractElem(id, name, descr)
{

}

Feature::~Feature()
{

}

QString Feature::GetInsertRequest()
{
    return (QString)"INSERT INTO Features (name, description) VALUES (" +
            "'" + _name + "'" + "," +
            "'" + _description + "'" +
            ")";
}

QString Feature::GetUpdateRequest()
{
    return (QString)"UPDATE Features SET " +
            "name = " + "'" + _name + "'" + ", "
            "description = " + "'" + _description + "'" + " "
            "WHERE id = " + QString::number(_id);
}
