#include "projectname.h"

ProjectName::ProjectName() : TableAbstractElem()
{

}

ProjectName::ProjectName(int id, const QString& projectNo, const QString &name, const QString& company, const QString& address, const QString &descr):
    TableAbstractElem(id, name, descr)
{
    _projectNo = projectNo;
    _companyName = company;
    _address = address;
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

QString ProjectName::GetAddress()
{
    return _address;
}

void ProjectName::SetAddress(const QString &value)
{
    _address = value;
}
