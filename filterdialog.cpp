#include "filterdialog.h"
#include "mainwindow.h"
#include <QLabel>

#define BTN_MEAN        0
#define BTN_MEDIAN      1
#define BTN_GAUSSIAN    2

FilterDialog::FilterDialog(QWidget *parent)
    :QDialog(parent)
{
    layout = new QVBoxLayout(this);

    QHBoxLayout* layout0 = new QHBoxLayout(this);
    krows = new QSpinBox(this);
    krows->setRange(1,20);
    krows->setSingleStep(1);
    krows->setValue(3);
    connect(krows,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    kcols = new QSpinBox(this);
    kcols->setRange(1,20);
    kcols->setSingleStep(1);
    kcols->setValue(3);
    connect(kcols,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    layout0->addWidget(new QLabel(tr("Kernel rows:")));
    layout0->addWidget(krows);
    layout0->addWidget(new QLabel(tr("Kernel cols:")));
    layout0->addWidget(kcols);
    layout->addLayout(layout0);
    QHBoxLayout* layout1 = new QHBoxLayout(this);
    anchorx = new QSpinBox(this);
    anchorx->setRange(0,19);
    anchorx->setSingleStep(1);
    anchorx->setValue(1);
    connect(anchorx,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    anchory = new QSpinBox(this);
    anchory->setRange(0,19);
    anchory->setSingleStep(1);
    anchory->setValue(1);
    connect(anchory,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    layout1->addWidget(new QLabel(tr("Anchor posx:")));
    layout1->addWidget(anchorx);
    layout1->addWidget(new QLabel(tr("Anchor posy:")));
    layout1->addWidget(anchory);
    layout->addLayout(layout1);
    variance = new QDoubleSpinBox(this);
    variance->setRange(0,10);
    variance->setSingleStep(0.01);
    variance->setValue(1);
    connect(variance,SIGNAL(valueChanged(double)),this,SLOT(on_value_changed()));
    QHBoxLayout* layout2 = new QHBoxLayout(this);
    layout2->addWidget(new QLabel(tr("Variance:")));
    layout2->addWidget(variance);
    layout->addLayout(layout2);

    group = new QButtonGroup(this);
    QHBoxLayout* layout3 = new QHBoxLayout(this);
    btn_mean = new QRadioButton(tr("Mean"),this);
    btn_mean->setChecked(true);
    btn_median = new QRadioButton(tr("Median"),this);
    btn_Gussian = new QRadioButton(tr("Gussian"),this);
    group->addButton(btn_mean, BTN_MEAN);
    group->addButton(btn_median, BTN_MEDIAN);
    group->addButton(btn_Gussian, BTN_GAUSSIAN);
    layout3->addWidget(btn_mean);
    layout3->addWidget(btn_median);
    layout3->addWidget(btn_Gussian);
    layout->addLayout(layout3);

    check_preview = new QCheckBox(tr("preview"),this);
    check_preview->setChecked(false);
    layout->addWidget(check_preview);

    button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(button);
    this->setLayout(layout);

    ImgWidget* img = ((MainWindow*)parent)->imgWidget;
    mat_show = new Mat(img->mat->rows, img->mat->cols,CV_8UC1);
    img->mat->copyTo(*mat_show);
}

FilterDialog::~FilterDialog()
{
    delete layout;
    delete group, button;
    delete btn_mean, btn_median, btn_Gussian;
    delete mat_show;
}

void FilterDialog::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    check_preview->setChecked(true);
    on_value_changed();
    img->updateImg(mat_show);
    QDialog::accept();
}

void FilterDialog::reject()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    img->showImg(img->mat);
    QDialog::reject();
}

void FilterDialog::on_value_changed()
{
    if(!check_preview->isChecked())return;
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    int type = group->checkedId();
    int kr = krows->value(), kc = kcols->value();
    int ax = anchorx->value(), ay = anchory->value();
    if(ay > kr || ax > kc)return;
    Mat* kernel;
    switch (type) {
    case BTN_MEAN:
        meanFilter(img->mat,mat_show,kr,kc,ax,ay);
        img->showImg(mat_show);
        break;
    case BTN_MEDIAN:
        medianFilter(img->mat,mat_show,kr,kc,ax,ay);
        img->showImg(mat_show);
        break;
    case BTN_GAUSSIAN:
        kernel = new Mat(kr, kc, CV_64FC1);
        calGaussianFilter(kernel,variance->value());
        GaussianFilter(img->mat,mat_show, kernel, ax, ay);
        img->showImg(mat_show);
        delete kernel;
        break;
    default: break;
    }
}
