#include "databasemanager.h"

#include <QDebug>

DatabaseManager::DatabaseManager()
{
    _db = QSqlDatabase::addDatabase("QODBC");
    _dateTimeFormat = "dd-MM-yyyy";
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

    bool result = _db.open();
    if (result)
    {
        if (!CheckTable("Previews"))
        {
            result = CreateTable("Previews");
        }
    }
    else
    {
        _db = QSqlDatabase::addDatabase("QSQLITE");
        //connection = "c:\\Work\\Peri\\periphotoproject\\build-PhotoManager-Desktop_Qt_5_7_0_MSVC2013_32bit-Debug\\PeriProjects.db";
        connection = "PeriProjects.db";
        _db.setDatabaseName(connection);
        result = _db.open();
        _dateTimeFormat = "yyyy-MM-dd";
    }

    QVector<Categorie> updateCats;
    updateCats.push_back(Categorie(1, "Chronology", ""));
    updateCats.push_back(Categorie(2, "Business photo, documentation", ""));
    result = InsertOrUpdateElems(updateCats);

    return result;
}

bool DatabaseManager::CreateTable(const QString &tableName)
{
    if (!_db.isOpen()) return false;
    QSqlQuery query;
    QString request = "CREATE TABLE " + tableName;
    request += " (";
    request += "ID INTEGER PRIMARY KEY IDENTITY(1,1), ";
    request += "PHOTOID INTEGER NOT NULL,";
    request += "FILEPATH VARCHAR(255) NOT NULL";
    request += ")";
    return query.exec(request);
}

bool DatabaseManager::InsertTestValuesToCategoriesTable()
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    bool result = query.exec("INSERT INTO Categories (name, description) VALUES('In process', '')");
    QString error = query.lastError().text();
    result &= query.exec("INSERT INTO Categories (name) VALUES('Current state')");
    error = query.lastError().text();
    result &= query.exec("INSERT INTO Categories (name) VALUES('Marketing')");
    error = query.lastError().text();

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

template<typename T> bool DatabaseManager::SelectElems(QVector<T> &elems, const QString &tableName, int elemId)
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    QString queryStr = "SELECT ID, Name, Description FROM " + tableName;
    if (elemId != -1)
    {
        queryStr += " WHERE ID = " + QString::number(elemId);
    }

    if (query.exec(queryStr))
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

template<typename T> bool DatabaseManager::SelectElemsFromPhotos(QVector<T> &elems, const QString &what, const QString& table, int photoId)
{
    bool result = false;

    QString sqlQuery = "SELECT " + what + " FROM Photos WHERE ID = " + QString::number(photoId);
    QSqlQuery query;

    if (query.exec(sqlQuery))
    {
        while (query.next())
        {
            QString data = query.value(what).toString();
            int pos = 0;
            int newPos = 0;
            do
            {
                newPos = data.indexOf(";", pos);
                QString elemId = "";
                if (newPos != -1)
                {
                    elemId = data.mid(pos, newPos - pos);
                }
                else
                {
                    elemId = data.mid(pos);
                }
                SelectElems(elems, table, elemId.toInt());
                pos = newPos + 1;
            }while (newPos != -1);
        }
        result = true;
    }

    qDebug() << sqlQuery;
    qDebug() << query.lastError().text();

    return result;
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
            QString error = query.lastError().text();
            qDebug() << error;
        }
        else
        {
            QString request = elem.GetInsertRequest();
            result &= query.exec(request);
            QString error = query.lastError().text();
            qDebug() << error;
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

bool DatabaseManager::SelectProjectNames(QVector<ProjectName> &elems)
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    if (query.exec("SELECT ID, ProjectNo, Name, Description FROM Projects"))
    {
        while (query.next())
        {
            int id = query.value("ID").toInt();
            QString projectNo = query.value("ProjectNo").toString();
            QString name = query.value("Name").toString();
            QString description = query.value("Description").toString();
            elems.push_back(ProjectName(id, projectNo, name, description));
        }
    }
    return true;
}

bool DatabaseManager::SelectProjectName(int projectId, QString &projectName)
{
    if (!_db.isOpen()) return false;

    QSqlQuery query;
    if (query.exec("SELECT Name FROM Projects WHERE ID = " + QString::number(projectId)))
    {
        while (query.next())
        {
            projectName = query.value("Name").toString();
        }
    }
    return true;
}

bool DatabaseManager::SelectPhotos(const QString &projectNo,
                                   const QString &projectName,
                                   const QDate &projectDate,
                                   const QVector<FormworkSystem> &formworkSystems,
                                   const QVector<Feature> &features,
                                   const QVector<Categorie> &categories,
                                   const QDate &intervalBegin,
                                   const QDate &intervalEnd,
                                   QVector<SearchResult> &photos)
{
    if (!_db.isOpen()) return false;

    bool result = false;

    QSqlQuery query;
    QString sqlQuery = "SELECT Photos.ID, FilePath, ProjectID, Time, Category, FormworkSystems, Features, Projects.Name  ";
    sqlQuery += "FROM Photos ";

    sqlQuery += "INNER JOIN Projects ";
    sqlQuery += "ON ";
    sqlQuery += QString("(ProjectNo LIKE ") + "'%" + projectNo + "%')";
    if (!projectName.isEmpty())
    {
        sqlQuery += " AND ";
        sqlQuery += QString("(Name LIKE ") + "'%" + projectName + "%')";
    }
    if (projectDate != QDate(1970, 1, 1))
    {
        sqlQuery += " AND ";
        sqlQuery += QString("(Projects.CreationTime = ") + "'" + projectDate.toString(_dateTimeFormat) + "')";
    }

    sqlQuery += " WHERE ";

    sqlQuery += " Projects.ID = Photos.ProjectID ";

    if (!formworkSystems.empty())
    {
        sqlQuery += " AND ";
        sqlQuery += " ( ";
    }

    for (int i = 0; i < formworkSystems.length(); i++)
    {
        if (i > 0)
        {
            sqlQuery += " OR ";
        }
        FormworkSystem system = formworkSystems[i];
        //alone ID
        sqlQuery += QString("FormworkSystems ") + "LIKE " + "'" + QString::number(system.GetID()) + "' ";
        sqlQuery += " OR ";
        //ID at the beginning
        sqlQuery += QString("FormworkSystems ") + "LIKE " + "'" + QString::number(system.GetID()) + ";%'";
        sqlQuery += " OR ";
        //ID in the middle
        sqlQuery += QString("FormworkSystems ") + "LIKE " + "'%;" + QString::number(system.GetID()) + ";%'";
        sqlQuery += " OR ";
        //ID in the end
        sqlQuery += QString("FormworkSystems ") + "LIKE " + "'%;" + QString::number(system.GetID()) + "'";
    }

    if (!formworkSystems.empty())
    {
        sqlQuery += " ) ";
    }

    if (!features.empty())
    {
        sqlQuery += " AND ";
        sqlQuery += " ( ";
    }

    for (int i = 0; i < features.length(); i++)
    {
        if (i > 0)
        {
            sqlQuery += " OR ";
        }
        Feature feature = features[i];
        //alone ID
        sqlQuery += QString("Features ") + "LIKE " + "'" + QString::number(feature.GetID()) + "' ";
        sqlQuery += " OR ";
        //ID at the beginning
        sqlQuery += QString("Features ") + "LIKE " + "'" + QString::number(feature.GetID()) + ";%'";
        sqlQuery += " OR ";
        //ID in the middle
        sqlQuery += QString("Features ") + "LIKE " + "'%;" + QString::number(feature.GetID()) + ";%'";
        sqlQuery += " OR ";
        //ID in the end
        sqlQuery += QString("Features ") + "LIKE " + "'%;" + QString::number(feature.GetID()) + "'";
    }

    if (!features.empty())
    {
        sqlQuery += " ) ";
    }

    if (!categories.empty())
    {
        sqlQuery += " AND ";
        sqlQuery += " ( ";
    }

    for (int i = 0; i < categories.length(); i++)
    {
        if (i > 0)
        {
            sqlQuery += " OR ";
        }
        Categorie cat = categories[i];
        //alone ID
        sqlQuery += QString("Category ") + "LIKE " + "'" + QString::number(cat.GetID()) + "' ";
        sqlQuery += " OR ";
        //ID in the middle
        sqlQuery += QString("Category ") + "LIKE " + "'%" + QString::number(cat.GetID()) + ";%'";
        sqlQuery += " OR ";
        //ID in the end
        sqlQuery += QString("Category ") + "LIKE " + "'%" + QString::number(cat.GetID()) + "'";
    }

    if (!categories.empty())
    {
        sqlQuery += " ) ";
    }

    if (intervalBegin != QDate(1970, 1, 1))
    {
        sqlQuery += " AND ";
        sqlQuery += " ( ";
        sqlQuery += QString(" Photos.Time >= ") + "'" + intervalBegin.toString(_dateTimeFormat) + "'";
    }

    if (intervalEnd != QDate(1970, 1, 1))
    {
        sqlQuery += " AND ";
        sqlQuery += QString(" Photos.Time <= ") + "'" + intervalEnd.toString(_dateTimeFormat) + "'";
    }

    if (intervalBegin != QDate(1970, 1, 1))
    {
        sqlQuery += " ) ";
    }


    if (query.exec(sqlQuery))
    {
        while (query.next())
        {
            SearchResult fnp;
            fnp.photoId = query.value("ID").toInt();
            fnp.filePath = query.value("FilePath").toString();
            QString time = query.value("Time").toString();
            QDate date = query.value("Time").toDate();
            fnp.photoDate = date;
            fnp.categories = query.value("Category").toString();
            fnp.projectName = query.value("Name").toString();
            photos.push_back(fnp);
        }
        result = true;
    }

    qDebug() << sqlQuery;
    qDebug() << query.lastError().text();

    SelectPreviews(photos);

    return result;
}

bool DatabaseManager::SelectPreviews(QVector<SearchResult> &photos)
{
    bool result = false;

    QString sqlQuery = "SELECT FilePath FROM Previews WHERE PhotoID = %1";
    QSqlQuery query;

    for (int i = 0; i < photos.length(); i++)
    {
        QString temp = sqlQuery.arg(photos[i].photoId);
        if (query.exec(temp))
        {
            while (query.next())
            {
                photos[i].previewPath = query.value("FilePath").toString();
            }
            result = true;
        }
    }

    qDebug() << sqlQuery;
    qDebug() << query.lastError().text();

    return result;
}

bool DatabaseManager::SelectFormworkSystemsByPhotoId(QVector<FormworkSystem> &elems, int photoId)
{
    return SelectElemsFromPhotos(elems, "FormworkSystems", "FormworkSystems", photoId);
}

bool DatabaseManager::SelectFeaturesByPhotoId(QVector<Feature> &elems, int photoId)
{
    return SelectElemsFromPhotos(elems, "Features", "Features", photoId);
}

bool DatabaseManager::SelectCategoriesByPhotoId(QVector<Categorie> &elems, int photoId)
{
    return SelectElemsFromPhotos(elems, "Category", "Categories", photoId);
}

bool DatabaseManager::InsertValuesToPhotos(const QString &projectNo,
                                           const QString &projectName,
                                           const QDate  &projectDate,
                                           const QString &formworkSystems,
                                           const QString &features,
                                           const QString &categories,
                                           const QVector<FileInfoStruct> &photos,
                                           const QVector<QFileInfo> &previews)
{
    int projectID = CheckProjectNo(projectNo);
    if (projectID == -1)
    {
        qDebug() << "projectID == -1";
        return false;
    }

    if (projectID == 0)
    {
        if (InsertProject(projectNo, projectName, projectDate, ""))
        {
            projectID = CheckProjectNo(projectNo);
        }
    }
    else
    {
        QString projectNameFromDB;
        SelectProjectName(projectID, projectNameFromDB);
        if (projectNameFromDB.isEmpty())
        {
            UpdateProjectName(projectID, projectName);
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
    queryString += "'%1', "; //for filelist
    queryString += "'" + formworkSystems + "', ";
    queryString += (!features.isEmpty() ? "'" + features + "', ": "");
    queryString += "'" + categories + "', ";
    queryString += "'%2'"; //for filelist
    queryString += ")";

    QSqlQuery query;
    bool result = true;
    for (int i = 0; i < photos.length(); i++)
    {
        FileInfoStruct file = photos[i];
        if (!file.fileInfo.filePath().isEmpty())
        {
            QString fileDate = file.lastModified.date().toString(_dateTimeFormat);
            QString temp = queryString.arg(fileDate, file.fileInfo.filePath());
            query.prepare(temp);
            qDebug() << temp;
            result = query.exec();
            if (result)
            {
                InsertPreview(query.lastInsertId().toInt(), previews[i]);
            }
            qDebug() << "insert result " << result << query.lastError().text();
        }
    }

    return true;
}

bool DatabaseManager::InsertPreview(int photoID, const QFileInfo &preview)
{
    QString queryString = "INSERT INTO Previews ";

    //column names
    queryString += "(";
    queryString += "PhotoID, ";
    queryString += "FilePath";
    queryString += ")";

    //values
    queryString += " VALUES ";
    queryString += "(";
    queryString += QString::number(photoID) + ", ";
    queryString += "'" + preview.filePath() + "'";
    queryString += ")";

    QSqlQuery query;
    bool result = query.exec(queryString);
    qDebug() << queryString << " result " << query.lastError().text();

    return result;
}

bool DatabaseManager::InsertProject(const QString &projectNo,
                                    const QString &name,
                                    const QDate &creationDate,
                                    const QString &description)
{
    if (!_db.isOpen()) return false;

    QString queryString = "INSERT INTO Projects ";

    //names
    queryString += "(";
    queryString += "ProjectNo";
    queryString += (!name.isEmpty() ? ", Name" : "");
    queryString += (!creationDate.isNull() ? ", CreationTime" : "");
    queryString += (!description.isEmpty() ? ", Descriptiom" : "");
    queryString += ")";

    queryString += " VALUES ";

    //values
    queryString += "(";
    queryString += "'" + projectNo + "'";
    queryString += (!name.isEmpty() ? ",'" + name + "'": "");
    queryString += (!creationDate.isNull() ? ",'" + creationDate.toString(_dateTimeFormat) + "'" : "");
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

bool DatabaseManager::CheckTable(const QString &tableName)
{
    if (!_db.isOpen()) return false;
    return _db.tables().contains(tableName);
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

bool DatabaseManager::UpdatePhotoAttributes(const QString &formworkSystems, const QString &features, const QString &categories, int photoId)
{
    if (!_db.isOpen()) return false;
    QString queryStr = (QString)"UPDATE Photos SET " +
            "FormworkSystems = " + "'" + formworkSystems + "'" + ", " +
            "Features = " + "'" + features + "'" + ", " +
            "Category = " + "'" + categories + "'" + " " +
            "WHERE ID = " + QString::number(photoId);

    QSqlQuery query;
    bool result = true;
    result &= query.exec(queryStr);

    qDebug() << queryStr;
    qDebug() << query.lastError().text();

    return result;
}

bool DatabaseManager::UpdateProjectName(int projectId, const QString &projectName)
{
    if (!_db.isOpen()) return false;

    QString queryStr = (QString)"UPDATE Projects SET " +
            "Name = " + "'" + projectName + "'" + " " +
            "WHERE ID = " + QString::number(projectId);

    QSqlQuery query;
    bool result = true;
    result &= query.exec(queryStr);

    qDebug() << queryStr;
    qDebug() << query.lastError().text();

    return result;
}

bool DatabaseManager::DeletePhoto(int photoID)
{
    if (!_db.isOpen()) return false;

    QString queryStr = (QString)"DELETE FROM Photos " +
            "WHERE ID = " + QString::number(photoID);

    QSqlQuery query;
    bool result = true;
    result &= query.exec(queryStr);

    qDebug() << queryStr;
    qDebug() << query.lastError().text();

    return result;
}

bool DatabaseManager::DeletePreview(int photoID)
{
    if (!_db.isOpen()) return false;

    QString queryStr = (QString)"DELETE FROM Previews " +
            "WHERE PHOTOID = " + QString::number(photoID);

    QSqlQuery query;
    bool result = true;
    result &= query.exec(queryStr);

    qDebug() << queryStr;
    qDebug() << query.lastError().text();

    return result;
}
