#include "categorie.h"

Categorie::Categorie() :
    TableAbstractElem()
{

}

Categorie::Categorie(int id, const QString& name, const QString& descr) :
    TableAbstractElem(id, name, descr)
{

}

Categorie::~Categorie()
{

}

QString Categorie::GetInsertRequest()
{
    return (QString)"INSERT INTO Categories (name, description) VALUES (" +
            "'" + _name + "'" + "," +
            "'" + _description + "'" +
            ")";
}

QString Categorie::GetUpdateRequest()
{
    return (QString)"UPDATE Categories SET " +
            "name = " + "'" + _name + "'" + ", "
            "description = " + "'" + _description + "'" + " "
            "WHERE id = " + QString::number(_id);
}
