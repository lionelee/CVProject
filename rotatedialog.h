#ifndef ROTATEDIALOG_H
#define ROTATEDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QDialogButtonBox>
#include "imgprocess.h"

class RotateDialog: public QDialog
{
    Q_OBJECT

public:
    RotateDialog(QWidget* parent = 0);
    ~RotateDialog();

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

#endif // ROTATEDIALOG_H
