#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTableWidgetItem>


#include "databasemanager.h"
#include "filemanager.h"
#include "configurator.h"
#include "filecopierthread.h"
#include "previewsession.h"
#include "photoloader.h"
#include "photosearcher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void on_pushButtonTestSQL_clicked();

    void on_pushButtonLoadPhoto_clicked();

    void on_pushButtonAddToDB_clicked();

    void on_comboBoxSystems_ModelItemChanged(QStandardItem *item);
    void on_comboBoxFeatures_ModelItemChanged(QStandardItem *item);
    void on_comboBoxSystemsSearch_ModelItemChanged(QStandardItem *item);
    void on_comboBoxFeaturesSearch_ModelItemChanged(QStandardItem *item);

    void on_tabWidgetSystem_currentChanged(int index);

    void on_pushButtonApplySystem_clicked();

    void on_tableWidgetSystems_itemChanged(QTableWidgetItem *item);

    void on_pushButtonSystemsNew_clicked();

    void on_pushButtonNewFeature_clicked();

    void on_pushButtonApplyFeature_clicked();

    void on_tableWidgetFeatures_itemChanged(QTableWidgetItem *item);

    void on_tabWidgetSystem_tabBarClicked(int index);

    void on_checkBoxEnablePreview_clicked();

    void on_comboBoxSystems_currentIndexChanged(int index);

    void on_lineEditProjectNo_textEdited(const QString &arg1);

    void on_lineEditProjectName_textEdited(const QString &arg1);

    void on_pushButtonSearch_clicked();

    void on_pushButtonSavePhotos_clicked();

    void on_checkBoxDisableProjectDate_clicked();

    void on_checkBoxDisableIntervalBegin_clicked();

    void on_checkBoxDisableIntervalEnd_clicked();

    void on_checkBoxEnablePreviewSearch_clicked();

private:
    void InitDatabase();
    void InitInterface();

    QString GetSelectedCategories();
    QVector<Categorie> GetSelectedCategoriesSearch();

    void ClearInterface(int tabIndex);

    void LoadInterface();
    void LoadDatabase();


    void AddFileToPreview(const QString& fileName);
    void DeleteFileFromPreview(int num);

    void EnableDranNDrop();

    bool ConfirmWindow();

    bool eventFilter(QObject *watched, QEvent *event);


public slots:
    void setProgressBarValue(int value);
    void finishedCopy();

private:
    Ui::MainWindow *ui;
    DatabaseManager* _dbm;

    ProjectName _backup;
    QStringList _files;
    QDate _filesDate;
    bool _dbChangesFlag;
    QVector<FormworkSystem> _selectedSystems;
    QVector<Feature> _selectedFeatures;
    QVector<FileAndPreview> _searchResult;

    FileManager* _fileManager;
    Configurator* _cfg;
    FilecopierThread* _copierThread;
    PreviewSession* _previewSession;

    PhotoLoader* _loader;
    PhotoSearcher* _searcher;
};

#endif // MAINWINDOW_H
