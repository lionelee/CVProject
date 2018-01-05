#ifndef GRAYMORPHOLOGY_H
#define GRAYMORPHOLOGY_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include "imgprocess.h"

#define GEROSION    0
#define GDILATION   1
#define GOPEN       2
#define GCLOSE      3
#define GEROSRBD    4
#define GDILARBD    5
#define GOPENRBD    6
#define GCLOSERBD   7

class GrayMorphology: public QDialog
{
    Q_OBJECT

public:
    GrayMorphology(QWidget* parent = 0, int _type = GEROSION);
    ~GrayMorphology();

protected:
    Mat* openFile();

private slots:
    void accept();
    void reject();

private:
    QVBoxLayout *layout;
    QLabel *label_row, *label_col, *label_iter;
    QSpinBox *spin_row, *spin_col, *spin_iter;
    QDialogButtonBox *button;
    int type;
};

#endif // GRAYMORPHOLOGY_H
