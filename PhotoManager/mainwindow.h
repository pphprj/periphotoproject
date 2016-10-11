#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTableWidgetItem>

#include "databasemanager.h"
#include "filemanager.h"
#include "configurator.h"
#include "filecopierthread.h"

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

    void on_tabWidgetSystem_currentChanged(int index);

    void on_pushButtonApplySystem_clicked();

    void on_tableWidgetSystems_itemChanged(QTableWidgetItem *item);

    void on_pushButtonSystemsNew_clicked();

    void on_pushButtonNewFeature_clicked();

    void on_pushButtonApplyFeature_clicked();

    void on_tableWidgetFeatures_itemChanged(QTableWidgetItem *item);

    void on_tabWidgetSystem_tabBarClicked(int index);

    void on_checkBoxEnablePreview_clicked();

private:
    QString GetProjectNo();
    QString GetProjectName();
    QDate GetProjectDate();
    QString GetSelectedCategories();
    QStringList& GetFileList();
    void GetFilesDate();

    void ClearComboboxChecked(QComboBox* comboBox);
    void ClearInterface();

    void LoadInterface();
    void LoadDatabase();

    void NewItem(QTableWidget* table);
    void AddFileToPreview(const QString& fileName);
    void DeleteFileFromPreview(int num);

    void EnableDranNDrop();

    bool ConfirmWindow();

    bool eventFilter(QObject *watched, QEvent *event);

    template <typename T> void FillCombobox(const QVector<T>& elems, const QComboBox* comboBox);
    template <typename T> void FillTableWidget(const QVector<T>& elems, const QTableWidget* table);

    template <typename T> QString CreateIDsList(const QVector<T>& elems);
    template <typename T> QString CreateNamesList(const QVector<T>& elems);
    template <typename T> QVector<T> GetSelectedListItems(const QVector<T>& elems, const QAbstractItemModel* model);

    template <typename T> void ApplyChanges(QVector<T>& elems, const QTableWidget* table);
    template <typename T> void ItemChanged(QVector<T>& elems, QTableWidgetItem* item, QTableWidget* table);
    template <typename T> void ShowSelection(const QVector<T>& elems, QComboBox* comboBox, QStandardItem* item);

public slots:
    void setProgressBarValue(int value);
    void finishedCopy();

private:
    Ui::MainWindow *ui;
    DatabaseManager* _dbm;
    QVector<FormworkSystem> _formworkSystems;
    QVector<Feature> _features;
    QVector<Categorie> _categories;
    QStringList _files;
    QDate _filesDate;
    bool _dbChangesFlag;

    FileManager* _fileManager;
    Configurator* _cfg;
    FilecopierThread* _copierThread;
};

#endif // MAINWINDOW_H
