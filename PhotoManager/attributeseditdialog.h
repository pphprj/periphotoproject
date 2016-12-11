#ifndef ATTRIBUTESEDITDIALOG_H
#define ATTRIBUTESEDITDIALOG_H

#include <QDialog>
#include <QStandardItem>

#include "databasemanager.h"
#include "photosearcher.h"

namespace Ui {
class AttributesEditDialog;
}

class AttributesEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttributesEditDialog(QWidget *parent = 0);
    ~AttributesEditDialog();

    void LoadSelected(PhotoSearcher* seacher, int photoId);
    QVector<Categorie> GetSelectedCategories();

private slots:

    void on_comboBoxSystems_ModelItemChanged(QStandardItem *item);
    void on_comboBoxFeatures_ModelItemChanged(QStandardItem *item);

    void on_buttonBox_accepted();

private:
    Ui::AttributesEditDialog *ui;

    QVector<FormworkSystem> _selectedSystems;
    QVector<Feature> _selectedFeatures;

    PhotoSearcher* _seacher;
    int _photoId;
};

#endif // ATTRIBUTESEDITDIALOG_H
