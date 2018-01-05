#ifndef NOISEDIALOG_H
#define NOISEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QBoxLayout>

class NoiseDialog: public QDialog
{
    Q_OBJECT

public:
    NoiseDialog(QWidget* parent);
    ~NoiseDialog();

private slots:
    void accept();
    void reject();
    void on_value_changed();

private:
    QVBoxLayout* layout;
    QHBoxLayout* hlayout;
    QSlider* slider;
    QLabel* label_number;
    QButtonGroup* group;
    QRadioButton *btn_salt, *btn_pepper;
    QCheckBox *check_preview;
    QDialogButtonBox *button;

};

#endif // NOISEDIALOG_H
