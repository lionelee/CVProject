#ifndef BINARYMORPHOLOGY_H
#define BINARYMORPHOLOGY_H

#include <QDialog>
#include <QBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDialogButtonBox>
#include "imgprocess.h"

#define EROSION     0
#define DILATION    1
#define OPEN        2
#define CLOSE       3
#define THINNING    4
#define THICKING    5
#define DISTRANS    6
#define SKELETON    7
#define SKERECON    8

class BinaryMorphology: public QDialog
{
    Q_OBJECT

public:
    BinaryMorphology(QWidget* parent = 0, int _type = EROSION);
    ~BinaryMorphology();

protected:
    bool getCustomKernel(Mat* kernel);
    void operation(Mat* kernel);

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
    int type;
};

#endif // BINARYMORPHOLOGY_H
