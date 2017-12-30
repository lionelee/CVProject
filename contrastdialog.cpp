#include "contrastdialog.h"
#include "mainwindow.h"
#include "imgwidget.h"

ContrastDialog::ContrastDialog(QWidget* parent)
    :QDialog(parent)
{
    layout = new QVBoxLayout(this);
    hist = new int[256];
    for(int i = 0; i < 256; ++i){
        hist[i] = 0;
    }
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent)->imgWidget);
    mat_show = new Mat(img->mat->rows, img->mat->cols, CV_8UC1);
    cpMatGray(img->mat, mat_show);
    getHistogram(img->mat, hist);
    histWidget = new HistogramWidget(this, hist);
    layout->addWidget(histWidget);

    slider_smin = new QSlider(Qt::Horizontal,this);
    slider_smin->setMinimum(0);
    slider_smin->setMaximum(255);
    layout->addWidget(slider_smin);
    connect(slider_smin,SIGNAL(valueChanged(int)),this,SLOT(on_slider_changed()));
    slider_smax = new QSlider(Qt::Horizontal,this);
    slider_smax->setMinimum(0);
    slider_smax->setMaximum(255);
    layout->addWidget(slider_smax);
    connect(slider_smax,SIGNAL(valueChanged(int)),this,SLOT(on_slider_changed()));

    slider_dmin = new QSlider(Qt::Horizontal,this);
    slider_dmin->setMinimum(0);
    slider_dmin->setMaximum(255);
    layout->addWidget(slider_dmin);
    slider_dmin->setVisible(false);
    connect(slider_dmin,SIGNAL(valueChanged(int)),this,SLOT(on_slider_changed()));
    slider_dmax = new QSlider(Qt::Horizontal,this);
    slider_dmax->setMinimum(0);
    slider_dmax->setMaximum(255);
    layout->addWidget(slider_dmax);
    slider_dmax->setVisible(false);
    connect(slider_dmax,SIGNAL(valueChanged(int)),this,SLOT(on_slider_changed()));

    coeff1 = new QDoubleSpinBox(this);
    coeff1->setRange(0.01,100);
    coeff1->setSingleStep(0.01);
    coeff1->setValue(1.0);
    layout->addWidget(coeff1);
    coeff1->setVisible(false);
    connect(coeff1,SIGNAL(valueChanged(double)),this,SLOT(on_spin_changed()));
    coeff2 = new QDoubleSpinBox(this);
    coeff2->setValue(1.0);
    coeff2->setRange(0.01,100);
    coeff2->setSingleStep(0.01);
    coeff2->setValue(1.0);
    layout->addWidget(coeff2);
    coeff2->setVisible(false);
    connect(coeff2,SIGNAL(valueChanged(double)),this,SLOT(on_spin_changed()));

    btn_lin = new QRadioButton(tr("Linear"),this);
    btn_lin->setChecked(true);
    connect(btn_lin,SIGNAL(clicked(bool)),this,SLOT(on_button_clicked()));
    btn_pie = new QRadioButton(tr("PieceWise"),this);
    connect(btn_pie,SIGNAL(clicked(bool)),this,SLOT(on_button_clicked()));
    btn_log = new QRadioButton(tr("Logarithm"),this);
    connect(btn_log,SIGNAL(clicked(bool)),this,SLOT(on_button_clicked()));
    btn_exp = new QRadioButton(tr("Exponent"),this);
    connect(btn_exp,SIGNAL(clicked(bool)),this,SLOT(on_button_clicked()));
    btn_equ = new QRadioButton(tr("Equalization"),this);
    connect(btn_equ,SIGNAL(clicked(bool)),this,SLOT(on_button_clicked()));
    group = new QButtonGroup(this);
    group->addButton(btn_lin, BTN_LINEAR);
    group->addButton(btn_pie, BTN_PIECEWISE);
    group->addButton(btn_log, BTN_LOGARITHM);
    group->addButton(btn_exp, BTN_EXPONENT);
    group->addButton(btn_equ, BTN_EQUALIZE);
    QHBoxLayout* layout0 = new QHBoxLayout(this);
    layout0->addWidget(btn_lin);
    layout0->addWidget(btn_pie);
    QHBoxLayout* layout1 = new QHBoxLayout(this);
    layout1->addWidget(btn_log);
    layout1->addWidget(btn_exp);
    layout->addLayout(layout0);
    layout->addLayout(layout1);
    layout->addWidget(btn_equ);

    check_preview = new QCheckBox(tr("preview"),this);
    check_preview->setChecked(true);
    layout->addWidget(check_preview);

    button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(button);
    this->setLayout(layout);
}

ContrastDialog::~ContrastDialog()
{
    delete layout, hist;
    delete slider_smin, slider_smax, slider_dmin, slider_dmax;
    delete coeff1, coeff2;
    delete check_preview, histWidget;
    delete btn_lin, btn_pie, btn_log, btn_exp, btn_equ;
}

void ContrastDialog::updateHistogram()
{
    for(int i = 0; i < 256; ++i){
        hist[i] = 0;
    }
    getHistogram(mat_show, hist);
    histWidget->updateHist(hist);
}

void ContrastDialog::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    check_preview->setChecked(true);
    on_spin_changed();
    on_slider_changed();
    img->updateImg(mat_show);
    QDialog::accept();
}

void ContrastDialog::reject()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    img->showImg(img->mat);
    QDialog::reject();
}

void ContrastDialog::on_button_clicked()
{
    int type = group->checkedId();
    int min = 0, max = 0;
    if(type == BTN_EQUALIZE){
        ImgWidget* img = ((MainWindow*)parent())->imgWidget;
        if(img == NULL)return;
        slider_smin->setVisible(false);
        slider_smax->setVisible(false);
        slider_dmin->setVisible(false);
        slider_dmax->setVisible(false);
        coeff1->setVisible(false);
        coeff2->setVisible(false);
        equalization(img->mat,mat_show);
        img->showImg(mat_show);
        updateHistogram();
        return;
    }
    switch(type){
    case BTN_LINEAR:
        slider_smin->setVisible(true);
        slider_smax->setVisible(true);
        slider_dmin->setVisible(false);
        slider_dmax->setVisible(false);
        coeff1->setVisible(false);
        coeff2->setVisible(false);
        slider_smin->setMinimum(0);
        slider_smax->setMaximum(255);
        break;
    case BTN_PIECEWISE:
        slider_smin->setVisible(true);
        slider_smax->setVisible(true);
        slider_dmin->setVisible(true);
        slider_dmax->setVisible(true);
        coeff1->setVisible(false);
        coeff2->setVisible(false);
        getGrayRange(mat_show, min, max);
        slider_smin->setMinimum(min);
        slider_smax->setMaximum(max);
        break;
    case BTN_LOGARITHM:
        slider_smin->setVisible(false);
        slider_smax->setVisible(false);
        slider_dmin->setVisible(false);
        slider_dmax->setVisible(false);
        coeff1->setVisible(true);
        coeff2->setVisible(false);
        break;
    case BTN_EXPONENT:
        slider_smin->setVisible(false);
        slider_smax->setVisible(false);
        slider_dmin->setVisible(false);
        slider_dmax->setVisible(false);
        coeff1->setVisible(true);
        coeff2->setVisible(true);
        break;
    default:break;
    }
}

void ContrastDialog::on_slider_changed()
{
    if(!check_preview->isChecked())return;
    int type = group->checkedId();
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    if(img == NULL)return;
    int smin=0, smax=0, dmin=0, dmax=0, tmp=0;
    switch(type){
    case BTN_LINEAR:
        smin = slider_smin->value(), smax = slider_smax->value();
        if(smin > smax){
            tmp = smax;
            smax = smin;
            smin = tmp;
        }
        if(!linearAdjust(img->mat, mat_show, smin, smax))return;
        break;
    case BTN_PIECEWISE:
        smin = slider_smin->value(), smax = slider_smax->value();
        if(smin > smax){
            tmp = smax;
            smax = smin;
            smin = tmp;
        }
        dmin = slider_dmin->value(), dmax = slider_dmax->value();
        if(dmin > dmax){
            tmp = dmax;
            dmax = dmin;
            dmin = tmp;
        }
        if(!piecewiseAdjust(img->mat, mat_show, smin, smax, dmin, dmax))return;
        break;
    default:return;
    }
    img->showImg(mat_show);
    updateHistogram();
}

void ContrastDialog::on_spin_changed()
{
    if(!check_preview->isChecked())return;
    int type = group->checkedId();
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    if(img == NULL)return;
    switch(type){
    case BTN_LOGARITHM:
        if(!logAdjust(img->mat, mat_show, coeff1->value()))return;
        break;
    case BTN_EXPONENT:
        if(!expAdjust(img->mat, mat_show, coeff1->value(), coeff2->value()))return;
        break;
    default:return;
    }
    img->showImg(mat_show);
    updateHistogram();
}
