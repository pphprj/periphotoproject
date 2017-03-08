#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTableWidgetItem>


#include "databasemanager.h"
#include "filemanager.h"
#include "configurator.h"

#include "additionpage.h"
#include "searchpage.h"
#include "editdbpage.h"

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

    void on_pushButtonLoadPhoto_clicked();

    void on_pushButtonAddToDB_clicked();

    void on_tabWidgetSystem_currentChanged(int index);

    void on_pushButtonApplySystem_clicked();

    void on_pushButtonSystemsNew_clicked();

    void on_pushButtonNewFeature_clicked();

    void on_pushButtonApplyFeature_clicked();

    void on_tabWidgetSystem_tabBarClicked(int index);

    void on_checkBoxEnablePreview_clicked();

    void on_comboBoxSystems_currentIndexChanged(int index);

    void on_pushButtonSearch_clicked();

    void on_pushButtonSavePhotos_clicked();

    void on_checkBoxDisableProjectDate_clicked();

    void on_checkBoxDisableIntervalBegin_clicked();

    void on_checkBoxDisableIntervalEnd_clicked();

    void on_pushButtonPrintSelected_clicked();

    void on_checkBoxOrderByName_clicked();

    void on_dateEditOnly_dateChanged(const QDate &date);

    void on_checkBoxFilterByDate_clicked();

    void on_tableWidgetPhotosSearch_doubleClicked(const QModelIndex &index);

    void on_actionE_xit_triggered();

    void on_action_About_triggered();

    void on_groupBoxCategoriesSearch_toggled(bool arg1);

    void on_groupBoxProjectSearch_toggled(bool arg1);

    void on_pushButtonDeleteSelected_clicked();

    void refreshProjectsSlot();

    void on_tableWidgetSearchResult_doubleClicked(const QModelIndex &index);

private:
    void InitDatabase();
    void InitInterface();

    void ClearInterface(int tabIndex);

    void LoadInterface();
    void LoadDatabase();

    bool ConfirmWindow();

    bool eventFilter(QObject *watched, QEvent *event);  

private:
    Ui::MainWindow *ui;
    DatabaseManager* _dbm;

    FileManager* _fileManager;
    Configurator* _cfg;

    AdditionPage* _addPage;
    SearchPage* _searchPage;
    EditDBPage* _editDBPage;
};

#endif // MAINWINDOW_H
