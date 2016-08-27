#include "formworksystem.h"

FormworkSystem::FormworkSystem() : TableAbstractElem()
{

}

FormworkSystem::FormworkSystem(int id, const QString& name, const QString& descr) :
    TableAbstractElem(id, name, descr)
{

}

FormworkSystem::~FormworkSystem()
{

}

QString FormworkSystem::GetInsertRequest()
{
    return (QString)"INSERT FormworkSystems (name, description) VALUES (" +
            "'" + _name + "'" + "," +
            "'" + _description + "'" +
            ")";
}

QString FormworkSystem::GetUpdateRequest()
{
    return (QString)"UPDATE FormworkSystems SET " +
            "name = " + "'" + _name + "'" + ", "
            "description = " + "'" + _description + "'" +" "
            "WHERE id = " + QString::number(_id);
}
