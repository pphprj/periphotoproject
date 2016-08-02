#ifndef FEATURES_H
#define FEATURES_H

#include "tableabstractelem.h"

class Feature : public TableAbstractElem
{
public:
    Feature();
    Feature(int id, const QString& name, const QString& descr);
    virtual ~Feature();
    virtual QString GetInsertRequest();
};

#endif // FEATURES_H
