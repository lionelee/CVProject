#include "cannydialog.h"
#include "mainwindow.h"

CannyDialog::CannyDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle(tr("Canny Edge Detection"));
    layout = new QVBoxLayout(this);

    QHBoxLayout* layout_low = new QHBoxLayout(this);
    spin_low = new QSpinBox(this);
    spin_low->setRange(1,255);
    label_low = new QLabel(tr("low threshold:"));
    layout_low->addWidget(label_low);
    layout_low->addWidget(spin_low);
    layout->addLayout(layout_low);
    connect(spin_low,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));

    QHBoxLayout* layout_high = new QHBoxLayout(this);
    spin_high = new QSpinBox(this);
    spin_high->setRange(1,500);
    label_high = new QLabel(tr("high threshold:"));
    layout_high->addWidget(label_high);
    layout_high->addWidget(spin_high);
    layout->addLayout(layout_high);
    connect(spin_high,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));

    QHBoxLayout* layout_spin = new QHBoxLayout(this);
    spin = new QSpinBox(this);
    spin->setRange(1,9);
    spin->setValue(3);
    spin->setSingleStep(2);
    label_spin = new QLabel(tr("kernel sizse:"));
    layout_spin->addWidget(label_spin);
    layout_spin->addWidget(spin);
    layout->addLayout(layout_spin);
    connect(spin,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));

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
    delete label_low, label_high, label_spin;
    delete spin_low, spin_high;
    delete spin, check_preview;
}

void CannyDialog::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    int low = spin_low->value();
    int high = spin_high->value();
    if(low >high){
        int tmp = low;
        low = high;
        high = tmp;
    }
    CannyDetect(img->mat, mat_show, low, high, spin->value());
    img->updateImg(mat_show);
    QDialog::accept();
}

void CannyDialog::reject()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    img->showImg(img->mat);
    QDialog::reject();
}

void CannyDialog::on_value_changed()
{
    if(!check_preview->isChecked())return;
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    int low = spin_low->value(), high = spin_high->value();

    if(low >high){
        int tmp = low;
        low = high;
        high = tmp;
    }
    if(!CannyDetect(img->mat, mat_show, low, high, spin->value()))return;
    img->showImg(mat_show);
}
