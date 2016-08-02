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
    return "INSERT FormworkSystems (name, description) VALUES (" +
            _name + "," +
            _description +
            ")";
}

