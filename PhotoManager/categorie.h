#ifndef CATEGORIE_H
#define CATEGORIE_H

#include "tableabstractelem.h"

class Categorie : public TableAbstractElem
{
public:
    Categorie();
    Categorie(int id, const QString& name, const QString& descr);
    virtual ~Categorie();
    virtual QString GetInsertRequest();
};

#endif // CATEGORIE_H
