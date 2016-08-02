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
    return "INSERT Categories (name, description) VALUES (" +
            _name + "," +
            _description +
            ")";
}
