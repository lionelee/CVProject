#ifndef CUSTOMFILTER_H
#define CUSTOMFILTER_H

#include <QDialog>
#include <QBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDialogButtonBox>
#include "imgprocess.h"



class CustomFilter: public QDialog
{
    Q_OBJECT

public:
    CustomFilter(QWidget* parent = 0);
    ~CustomFilter();

protected:
    bool getCustomKernel(Mat* kernel);

private slots:
    void accept();
    void reject();
    void on_btn_clicked();

private:
    QVBoxLayout* layout;
    QGridLayout* grid;
    QTextEdit* text;
    QSpinBox* spin[16];
    QSpinBox *anchorx, *anchory;
    QButtonGroup* group;
    QRadioButton *btn_two, *btn_three, *btn_four, *btn_custom;
    QDialogButtonBox* button;
};


#endif // CUSTOMFILTER_H
