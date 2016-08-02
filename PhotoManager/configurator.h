#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QString>

class Configurator
{
public:
    Configurator();

    QString GetHost();
    QString GetUsername();
    QString GetPassword();
    QString GetProjectsDirectory();
private:
    QString _host;
    QString _username;
    QString _password;
    QString _projectsDirectory;
};

#endif // CONFIGURATOR_H
