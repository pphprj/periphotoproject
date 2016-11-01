#include "configurator.h"

#include <QSettings>
#include <QApplication>
#include <QTextCodec>

Configurator::Configurator()
{
    QString settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);

    settings.setIniCodec("Windows-1251");

    QStringList l = settings.allKeys();

    settings.beginGroup("DBConnection");
    _host = settings.value("Host").toString();
    _username = settings.value("Username").toString();
    _password = settings.value("Password").toString();
    settings.endGroup();

    settings.beginGroup("Files");
    _projectsDirectory = settings.value("Directory").toString();
    _lastFolder = settings.value("LastFolder").toString();
    _compressionRate = settings.value("CompressionRate").toInt();
    settings.endGroup();
}

Configurator::~Configurator()
{
    QString settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);
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
    return _projectsDirectory;
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
