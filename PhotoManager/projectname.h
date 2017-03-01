#ifndef PROJECTNAME_H
#define PROJECTNAME_H

#include "tableabstractelem.h"

class ProjectName : public TableAbstractElem
{
public:
    explicit ProjectName();
    ProjectName(int id, const QString& projectNo, const QString& name, const QString& company, const QString& descr);
    virtual ~ProjectName();
    virtual QString GetInsertRequest();

    QString GetProjectNo();
    QString GetCompanyName();
    void SetProjectNo(const QString& value);
    void SetCompanyName(const QString& value);
private:
    QString _projectNo;
    QString _companyName;
};

#endif // PROJECTNAME_H
