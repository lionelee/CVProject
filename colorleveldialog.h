#ifndef COLORLEVELDIALOG_H
#define COLORLEVELDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include "imgprocess.h"

class ColorLevelDialog: public QDialog
{
    Q_OBJECT

public:
    ColorLevelDialog(QWidget* parent = 0);
    ~ColorLevelDialog();

private slots:
    void accept();
    void reject();
    void on_value_changed();

private:
    QVBoxLayout* layout;
    QComboBox* combobox;
    QLabel *label_ib, *label_ig, *label_iw, *label_ob, *label_ow;
    QSlider *in_black, *in_gray, *in_white;
    QSlider *out_black, *out_white;
    QCheckBox *check_preview;
    QDialogButtonBox* button;

    Mat* mat_show;
};

#endif // COLORLEVELDIALOG_H
