#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QDialogButtonBox>
#include "imgprocess.h"

class ScaleDialog: public QDialog
{
    Q_OBJECT

public:
    ScaleDialog(QWidget* parent = 0);
    ~ScaleDialog();

private slots:
    void accept();
    void reject();
    void on_value_changed();

private:
    QVBoxLayout *layout;
    QHBoxLayout *layout1;
    QComboBox* combobox;
    QSlider* slider;
    QCheckBox* check_preview;
    QDialogButtonBox* button;
    QLabel* label;
};

#endif // SCALEDIALOG_H
