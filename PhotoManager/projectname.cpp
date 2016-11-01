#include "projectname.h"

ProjectName::ProjectName() : TableAbstractElem()
{

}

ProjectName::ProjectName(int id, const QString& projectNo, const QString &name, const QString &descr):
    TableAbstractElem(id, name, descr)
{
    _projectNo = projectNo;
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
