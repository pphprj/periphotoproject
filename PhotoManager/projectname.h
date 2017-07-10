#ifndef PROJECTNAME_H
#define PROJECTNAME_H

#include "tableabstractelem.h"

class ProjectName : public TableAbstractElem
{
public:
    explicit ProjectName();
    ProjectName(int id, const QString& projectNo, const QString& name, const QString& company, const QString& address, const QString& descr);
    virtual ~ProjectName();
    virtual QString GetInsertRequest();

    QString GetProjectNo();
    QString GetCompanyName();
    QString GetAddress();
    void SetProjectNo(const QString& value);
    void SetCompanyName(const QString& value);
    void SetAddress(const QString& value);
private:
    QString _projectNo;
    QString _companyName;
    QString _address;
};

#endif // PROJECTNAME_H
