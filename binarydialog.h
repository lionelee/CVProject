#ifndef BINARYDIALOG_H
#define BINARYDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QDialogButtonBox>
#include "histogramwidget.h"

class BinaryDialog: public QDialog
{
    Q_OBJECT

public:
    explicit BinaryDialog(QWidget *parent = 0);
    ~BinaryDialog();

private slots:
    void accept();
    void reject();
    void on_value_changed();

private:
    QLabel *label_min, *label_max;
    QSlider *slider_min, *slider_max;
    QCheckBox *check_preview;
    HistogramWidget *histWidget;
    QDialogButtonBox *button;
};

#endif // BINARYDIALOG_H
