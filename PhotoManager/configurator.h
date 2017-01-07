#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QString>

class Configurator
{
public:
    Configurator();
    ~Configurator();

    QString GetHost();
    QString GetUsername();
    QString GetPassword();
    QString GetProjectsDirectory();
    QString GetLastFolder();
    int GetCompressionRate();
    bool ShowEditDBTab();

    void SetLastFolder(const QString& value);
private:
    QString _host;
    QString _username;
    QString _password;
    QString _projectsDirectory;
    QString _lastFolder;
    int _compressionRate;
    bool _editDBTab;
};

#endif // CONFIGURATOR_H
