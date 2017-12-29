#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QCheckBox>
#include "imgwidget.h"

class FilterDialog: public QDialog
{
    Q_OBJECT

public:
    FilterDialog(QWidget* parent = 0);
    ~FilterDialog();

private slots:
    void accept();
    void reject();
    void on_value_changed();

private:
    QVBoxLayout* layout;
    QDialogButtonBox* button;
    QCheckBox *check_preview;
    QButtonGroup* group;
    QRadioButton *btn_mean, *btn_median, *btn_Gussian;
    QSpinBox *krows, *kcols, *anchorx, *anchory;
    QDoubleSpinBox *variance;
    Mat* mat_show;
};

#endif // FILTERDIALOG_H
