#ifndef PROJECTNAME_H
#define PROJECTNAME_H

#include "tableabstractelem.h"

class ProjectName : public TableAbstractElem
{
public:
    explicit ProjectName();
    ProjectName(int id, const QString& projectNo, const QString& name, const QString& descr);
    virtual ~ProjectName();
    virtual QString GetInsertRequest();

    QString GetProjectNo();
    void SetProjectNo(const QString& value);
private:
    QString _projectNo;
};

#endif // PROJECTNAME_H
