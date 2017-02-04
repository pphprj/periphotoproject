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
    QString GetRemoteDirectory();
    QString GetLastFolder();
    QString GetDeletePassword();
    int GetCompressionRate();
    bool ShowEditDBTab();
    bool ShowDeleteButton();

    void SetLastFolder(const QString& value);
private:
    QString _host;
    QString _username;
    QString _password;
    QString _projectsDirectory;
    QString _remoteProjectsDirectory;
    QString _lastFolder;
    int _compressionRate;
    bool _editDBTab;
    bool _deleteButton;
    QString _deletePassword;
};

#endif // CONFIGURATOR_H
