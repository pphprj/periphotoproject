#include "configurator.h"

#include <QSettings>
#include <QApplication>
#include <QTextCodec>
#include <QDebug>

Configurator::Configurator()
{
    QString settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);

    settings.setIniCodec("Windows-1251");

    settings.beginGroup("DBConnection");
    _host = settings.value("Host").toString();
    _username = settings.value("Username").toString();
    _password = settings.value("Password").toString();
    settings.endGroup();

    settings.beginGroup("Files");
    _projectsDirectory = settings.value("Directory").toString();
    qDebug() << "prject dir " << _projectsDirectory;
    _remoteProjectsDirectory = settings.value("RemoteDirectory").toString();
    qDebug() << "remote dir " << _remoteProjectsDirectory;
    _lastFolder = settings.value("LastFolder").toString();
    qDebug() << "last folder " << _lastFolder;
    _compressionRate = settings.value("CompressionRate").toInt();
    qDebug() << "compression rate " << _compressionRate;
    settings.endGroup();

    settings.beginGroup("Interface");
    _editDBTab = settings.value("EditDBTab", true).toBool();
    qDebug() << "edit DB tab " << _editDBTab;
    settings.endGroup();
}

Configurator::~Configurator()
{
    QString settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);

    settings.setIniCodec("Windows-1251");

    settings.beginGroup("Files");
    settings.setValue("LastFolder", _lastFolder);
    settings.endGroup();
}

QString Configurator::GetHost()
{
    return _host;
}

QString Configurator::GetUsername()
{
    return _username;
}

QString Configurator::GetPassword()
{
    return _password;
}

QString Configurator::GetProjectsDirectory()
{
    QString result = (_remoteProjectsDirectory.isEmpty()) ? _projectsDirectory : _remoteProjectsDirectory;
    return result;
}

QString Configurator::GetRemoteDirectory()
{
    return _remoteProjectsDirectory;
}

QString Configurator::GetLastFolder()
{
    return _lastFolder;
}

void Configurator::SetLastFolder(const QString &value)
{
    _lastFolder = value;
}

int Configurator::GetCompressionRate()
{
    return _compressionRate;
}

bool Configurator::ShowEditDBTab()
{
    return _editDBTab;
}
