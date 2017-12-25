#ifndef HSLDIALOG_H
#define HSLDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QCheckBox>
#include "imgprocess.h"

class HSLDialog: public QDialog
{
    Q_OBJECT

public:
    explicit HSLDialog(QWidget *parent = 0);
    ~HSLDialog();

private slots:
    void accept();
    void reject();
    void on_value_changed(int);

private:
    QSlider *slider_hue, *slider_sat, *slider_lig;
    QCheckBox *check_preview;

    Mat* mat_hsl;
};

#endif // HSLDIALOG_H
