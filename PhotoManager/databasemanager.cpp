#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
    _db = QSqlDatabase::addDatabase("QODBC");
}

DatabaseManager::~DatabaseManager()
{
    if (_db.isOpen())
    {
        _db.close();
    }
}

bool DatabaseManager::Connect(const QString& host, const QString& username, const QString& password)
{
    //db.setDatabaseName("DRIVER={SQL Native Client};Server=SAMSUNG-ПК\\SQLSERVER2005;Database=Orion15;Uid=sa;Port=1433;Pwd=12345;WSID=");
    //db.setDatabaseName("DRIVER={SQL Server};Server=SAMSUNG-ПК\\SQLSERVER2005;Database=Orion15;Uid=sa;Port=1433;Pwd=12345;WSID=");
     //    db.setDatabaseName(SAMSUNG-ПК\\SQLSERVER2005 Uid=sa;Port=1433;Pwd=12345;WSID=");

    QString connection = QString("DRIVER={SQL Server};") +
            "Server=" + host + ";" +
            "Database=PeriProjects;" +
            "Uid=" + username + ";" +
            "Port=1433;" +
            "Pwd=" + password + ";" +
            "WSID=";
    _db.setDatabaseName(connection);
    return _db.open();
}

bool DatabaseManager::InsertTestValuesToCategoriesTable()
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    bool result = query.exec("INSERT INTO Categories (name, description) VALUES('In process', '')");
    result &= query.exec("INSERT INTO Categories (name) VALUES('Current state')");
    result &= query.exec("INSERT INTO Categories (name) VALUES('Marketing')");

    return result;
}

bool DatabaseManager::InsertTestValuesToFeaturesTable()
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    bool result = query.exec("INSERT INTO Features (name) VALUES('Wall')");
    result &= query.exec("INSERT INTO Features (name) VALUES('Column')");
    result &= query.exec("INSERT INTO Features (name) VALUES('Slab')");
    result &= query.exec("INSERT INTO Features (name) VALUES('Shoring')");
    result &= query.exec("INSERT INTO Features (name) VALUES('Accessories')");

    return result;
}

bool DatabaseManager::InsertTestValuesToFormworkSystemsTable()
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    bool result = query.exec("INSERT INTO FormworkSystems (name) VALUES('DOMINO')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('DUO')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('LIWA')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('LICO')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('QUATTRO')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('RAPID')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('GRIDFLEX')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('SKYDECK')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('TABLE MODULE')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('HD 200')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('PD 8')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('PERI UP')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('PERI Elevating Platform 290')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('PTL 1250-2 Table Lift')");
    result &= query.exec("INSERT INTO FormworkSystems (name) VALUES('VT 20K Formwork Girder')");


    return result;
}

template<typename T> bool DatabaseManager::SelectElems(QVector<T> &elems, const QString &tableName)
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    if (query.exec("SELECT * FROM " + tableName))
    {
        while (query.next())
        {
            int id = query.value("ID").toInt();
            QString name = query.value("Name").toString();
            QString description = query.value("Description").toString();
            elems.push_back(T(id, name, description));
        }
    }
    return true;
}

template<typename T> bool DatabaseManager::InsertOrUpdateElems(const QVector<T>& elems)
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    bool result = true;
    for (int i = 0; i < elems.length(); i++)
    {
        T elem = elems[i];
        if (elem.GetID() != 0)
        {
            QString request = elem.GetUpdateRequest();
            result &= query.exec(request);
        }
        else
        {
            QString request = elem.GetInsertRequest();
            result &= query.exec(request);
        }
    }

    return result;
}

bool DatabaseManager::SelectFormworkSystems(QVector<FormworkSystem> &elems)
{
    return SelectElems(elems, "FormworkSystems");
}

bool DatabaseManager::SelectFeatures(QVector<Feature> &elems)
{
    return SelectElems(elems, "Features");
}

bool DatabaseManager::SelectCategories(QVector<Categorie> &elems)
{
    return SelectElems(elems, "Categories");
}

bool DatabaseManager::InsertValuesToPhotos(const QString &projectNo,
                                           const QDateTime &time,
                                           const QString &formworkSystems,
                                           const QString &features,
                                           const QString &categories,
                                           const QStringList &photos)
{
    int projectID = CheckProjectNo(projectNo);
    if (projectID == -1)
    {
        return false;
    }

    if (projectID == 0)
    {
        if (InsertProject(projectNo, "", time, ""))
        {
            projectID = CheckProjectNo(projectNo);
        }
    }

    QString queryString = "INSERT INTO Photos ";

    //column names
    queryString += "(";
    queryString += "ProjectID, ";
    queryString += "Time, ";
    queryString += "FormworkSystems, ";
    queryString += (!features.isEmpty() ? "Features, " : "");
    queryString += "Category, ";
    queryString += "FilePath";
    queryString += ")";

    //values
    queryString += " VALUES ";
    queryString += "(";
    queryString += QString::number(projectID) + ", ";
    queryString += "'" + time.toString("yyyy-MM-dd hh:mm:ss") + "', ";
    queryString += "'" + formworkSystems + "', ";
    queryString += (!features.isEmpty() ? "'" + features + "', ": "");
    queryString += "'" + categories + "', ";
    queryString += "'%1'"; //for filelist
    queryString += ")";

    QSqlQuery query;
    bool result = true;
    for (int i = 0; i < photos.length(); i++)
    {
        QString filepath = photos.at(i);
        if (!filepath.isEmpty())
        {
            QString temp = queryString.arg(filepath);
            result &= query.exec(temp);
        }
    }

    return result;
}

bool DatabaseManager::InsertProject(const QString &projectNo,
                                    const QString &name,
                                    const QDateTime &creationTime,
                                    const QString &description)
{
    if (!_db.isOpen()) return false;

    QString queryString = "INSERT INTO Projects ";

    //names
    queryString += "(";
    queryString += "ProjectNo";
    queryString += (!name.isEmpty() ? ", Name" : "");
    queryString += (!creationTime.isNull() ? ", CreationTime" : "");
    queryString += (!description.isEmpty() ? ", Descriptiom" : "");
    queryString += ")";

    queryString += " VALUES ";

    //values
    queryString += "(";
    queryString += "'" + projectNo + "'";
    queryString += (!name.isEmpty() ? ",'" + name + "'": "");
    queryString += (!creationTime.isNull() ? ",'" + creationTime.toString("yyyy-MM-dd hh:mm:ss") + "'" : "");
    queryString += (!description.isEmpty() ? ",'" + description + "'" : "");
    queryString += ")";

    QSqlQuery query;
    if (query.exec(queryString))
    {
        query.next();
        int id = query.value("ID").toInt();
        id = -1;
        return true;
    }
    return false;
}

int DatabaseManager::CheckProjectNo(const QString &projectNo)
{
    if (!_db.isOpen()) return -1;

    int result = -1;

    QSqlQuery query;
    if (query.exec("SELECT ID FROM Projects WHERE ProjectNo= '"  + projectNo + "'"))
    {
        query.next();
        result = query.value("ID").toInt();
    }
    return result;
}

bool DatabaseManager::UpdateFeatures(const QVector<Feature> &elems)
{
    if (!_db.isOpen()) return false;

    InsertOrUpdateElems(elems);

    return true;
}

bool DatabaseManager::UpdateFormworkSystems(const QVector<FormworkSystem> &elems)
{
    if (!_db.isOpen()) return false;

    InsertOrUpdateElems(elems);

    return true;
}
