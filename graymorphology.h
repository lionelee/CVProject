#ifndef GRAYMORPHOLOGY_H
#define GRAYMORPHOLOGY_H

#include <QDialog>
#include <QBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDialogButtonBox>
#include "imgprocess.h"

#define GEROSION    0
#define GDILATION   1
#define GOPEN       2
#define GCLOSE      3
#define GMORRECON   4
#define WATERSHED   5

class GrayMorphology: public QDialog
{
    Q_OBJECT

public:
    GrayMorphology(QWidget* parent = 0, int _type = GEROSION);
    ~GrayMorphology();

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

#endif // GRAYMORPHOLOGY_H
