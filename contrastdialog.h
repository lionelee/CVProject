#ifndef CONTRASTDIALOG_H
#define CONTRASTDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDialogButtonBox>
#include "histogramwidget.h"
#include "imgprocess.h"

#define BTN_LINEAR      0
#define BTN_PIECEWISE   1
#define BTN_LOGARITHM   2
#define BTN_EXPONENT    3
#define BTN_EQUALIZE    4

class ContrastDialog: public QDialog
{
    Q_OBJECT

public:
    explicit ContrastDialog(QWidget *parent = 0);
    ~ContrastDialog();

protected:
    void updateHistogram();

private slots:
    void accept();
    void reject();
    void on_button_clicked();
    void on_slider_changed();
    void on_spin_changed();

private:
    QVBoxLayout* layout;
    QSlider *slider_smin, *slider_smax, *slider_dmin, *slider_dmax;
    QCheckBox *check_preview;
    HistogramWidget *histWidget;
    QDoubleSpinBox *coeff1, *coeff2;
    QButtonGroup *group;
    QRadioButton *btn_lin, *btn_pie ,*btn_log, *btn_exp, *btn_equ;
    QDialogButtonBox* button;
    Mat* mat_show;
    int* hist;
};

#endif // CONTRASTDIALOG_H
