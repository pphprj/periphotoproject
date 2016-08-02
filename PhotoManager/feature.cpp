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
    return "INSERT Features (name, description) VALUES (" +
            _name + "," +
            _description +
            ")";
}
