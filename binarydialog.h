#ifndef BINARYDIALOG_H
#define BINARYDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QCheckBox>
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
    void on_value_changed(int);

private:
    QSlider *slider_min, *slider_max;
    QCheckBox *check_preview;
    HistogramWidget *histWidget;
};

#endif // BINARYDIALOG_H
