#ifndef ADDITIONPAGE_H
#define ADDITIONPAGE_H

#include <QProgressBar>

#include "page.h"
#include "filecopierthread.h"
#include "previewsession.h"


class AdditionPage : public Page
{
    Q_OBJECT
public:
    explicit AdditionPage(DatabaseManager* databaseManager, FileManager* fileManager, Configurator* cfg, QObject *parent = 0);

    ~AdditionPage();

    virtual void CreateLoader();
    virtual void ClearInterface();

    void InitAdditionPageInterface(QListWidget* photoList, QProgressBar* progess);

    void AddFileToPreview(const QString& fileName);
    void DeleteFileFromPreview(int num);

    void AddFilesToDatabase();

public slots:
    void setProgressBarValue(int value);
    void finishedCopy();

private:
    FilecopierThread* _copierThread;
    PreviewSession* _previewSession;
    QListWidget* _photoList;
    QProgressBar* _progress;

    QStringList _files;
};

#endif // ADDITIONPAGE_H
