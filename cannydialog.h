#ifndef CANNYDIALOG_H
#define CANNYDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QCheckBox>
#include "imgwidget.h"

class CannyDialog: public QDialog
{
    Q_OBJECT

public:
    CannyDialog(QWidget* parent = 0);
    ~CannyDialog();

private slots:
    void accept();
    void reject();
    void on_value_changed();

private:
    QVBoxLayout* layout;
    QDialogButtonBox* button;
    QSpinBox *spin_low, *spin_high, *spin;
    QCheckBox *check_preview;
    Mat* mat_show;
};

#endif // CANNYDIALOG_H
