#ifndef CUTDIALOG_H
#define CUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include "imgprocess.h"

class CutDialog: public QDialog
{
    Q_OBJECT

public:
    CutDialog(QWidget* parent = 0);
    ~CutDialog();

private slots:
    void accept();
    void reject();
    void on_value_changed();

private:
    QVBoxLayout* layout;
    QLabel *label_px, *label_py, *label_cw, *label_ch;
    QSlider *px, *py, *cw, *ch;
    QDialogButtonBox* button;
    QCheckBox *check_preview;

};

#endif // CUTDIALOG_H
