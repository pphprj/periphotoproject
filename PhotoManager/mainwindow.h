#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

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

private slots:
    void on_pushButtonTestSQL_clicked();

    void on_pushButtonLoadPhoto_clicked();

    void on_pushButtonAddToDB_clicked();

    void on_comboBoxSystems_ModelItemChanged(QStandardItem *item);
    void on_comboBoxFeatures_ModelItemChanged(QStandardItem *item);

private:
    QString GetProjectNo();
    QDateTime GetDateTime();
    QString GetSelectedCategories();
    QStringList& GetFileList();

    void ClearComboboxChecked(const QAbstractItemModel* model);
    void ClearInterface();

    template <typename T> void FillCombobox(const QVector<T>& elems, const QComboBox* comboBox);

    template <typename T> QString CreateIDsList(const QVector<T>& elems);
    template <typename T> QString GetSelectedListItems(const QVector<T>& elems, const QAbstractItemModel* model);

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
