#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTableWidgetItem>

#include "databasemanager.h"
#include "filemanager.h"
#include "configurator.h"

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

private:
    QString GetProjectNo();
    QDateTime GetDateTime();
    QString GetSelectedCategories();
    QStringList& GetFileList();

    void ClearComboboxChecked(const QAbstractItemModel* model);
    void ClearInterface();

    void LoadInterface();
    void LoadDatabase();

    void NewItem(QTableWidget* table);

    void EnableDranNDrop();

    bool ConfirmWindow();

    template <typename T> void FillCombobox(const QVector<T>& elems, const QComboBox* comboBox);
    template <typename T> void FillTableWidget(const QVector<T>& elems, const QTableWidget* table);

    template <typename T> QString CreateIDsList(const QVector<T>& elems);
    template <typename T> QString GetSelectedListItems(const QVector<T>& elems, const QAbstractItemModel* model);

    template <typename T> void ApplyChanges(QVector<T>& elems, const QTableWidget* table);
    template <typename T> void ItemChanged(QVector<T>& elems, QTableWidgetItem* item, QTableWidget* table);


private:
    Ui::MainWindow *ui;
    DatabaseManager* _dbm;
    QVector<FormworkSystem> _formworkSystems;
    QVector<Feature> _features;
    QVector<Categorie> _categories;
    QStringList _files;

    Filemanager* _fileManager;
    Configurator* _cfg;
};

#endif // MAINWINDOW_H
