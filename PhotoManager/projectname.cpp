#include "projectname.h"

ProjectName::ProjectName() : TableAbstractElem()
{

}

ProjectName::ProjectName(int id, const QString& projectNo, const QString &name, const QString& company, const QString &descr):
    TableAbstractElem(id, name, descr)
{
    _projectNo = projectNo;
    _companyName = company;
}

ProjectName::~ProjectName()
{

}

QString ProjectName::GetInsertRequest()
{
    return "";
}

QString ProjectName::GetProjectNo()
{
    return _projectNo;
}

void ProjectName::SetProjectNo(const QString &value)
{
    _projectNo = value;
}

QString ProjectName::GetCompanyName()
{
    return _companyName;
}

void ProjectName::SetCompanyName(const QString &value)
{
    _companyName = value;
}
