#include "cannydialog.h"
#include "mainwindow.h"

CannyDialog::CannyDialog(QWidget *parent)
    :QDialog(parent)
{
    layout = new QVBoxLayout(this);

    slider_low = new QSlider(Qt::Horizontal,this);
    slider_low->setRange(0,255);
    connect(slider_low,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    slider_high = new QSlider(Qt::Horizontal,this);
    slider_high->setRange(0,255);
    connect(slider_high,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    layout->addWidget(slider_low);
    layout->addWidget(slider_high);

    spin = new QSpinBox(this);
    spin->setRange(1,9);
    spin->setValue(3);
    spin->setSingleStep(2);
    connect(spin,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    layout->addWidget(spin);

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

CannyDialog::~CannyDialog()
{
    delete layout, button;
    delete slider_low, slider_high;
    delete spin, check_preview;
    delete mat_show;
}

void CannyDialog::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    int low = slider_low->value();
    int high = slider_high->value();
    if(low >high){
        int tmp = low;
        low = high;
        high = tmp;
    }
    CannyDetect(img->mat, mat_show, low, high, spin->value());
    QDialog::accept();
}

void CannyDialog::reject()
{
    QDialog::reject();
}

void CannyDialog::on_value_changed()
{
    if(!check_preview->isChecked())return;
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    int low = slider_low->value();
    int high = slider_high->value();
    if(low >high){
        int tmp = low;
        low = high;
        high = tmp;
    }
    if(!CannyDetect(img->mat, mat_show, low, high, spin->value()))return;
    img->showImg(mat_show);
}
