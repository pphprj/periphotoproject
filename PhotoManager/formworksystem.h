#ifndef FORMWORKSYSTEM_H
#define FORMWORKSYSTEM_H

#include "tableabstractelem.h"

class FormworkSystem : public TableAbstractElem
{
public:
    FormworkSystem();
    FormworkSystem(int id, const QString& name, const QString& descr);
    virtual ~FormworkSystem();

    virtual QString GetInsertRequest();
    virtual QString GetUpdateRequest();
};

#endif // FORMWORKSYSTEM_H
