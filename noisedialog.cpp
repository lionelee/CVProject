#include "noisedialog.h"
#include "mainwindow.h"
#include "imgwidget.h"
#include <QLabel>

#define BTN_SALT        0
#define BTN_PEPPER      1
#define BTN_GAUSSIAN    2

NoiseDialog::NoiseDialog(QWidget *parent)
    :QDialog(parent)
{
    layout = new QVBoxLayout(this);
    hlayout = new QHBoxLayout(this);
    ImgWidget* img = ((MainWindow*)parent)->imgWidget;
    Mat* src = img->mat;

    QHBoxLayout* layout0 = new QHBoxLayout(this);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, src->rows*src->cols);
    slider->setValue(0);
    layout0->addWidget(new QLabel(tr("number:")));
    layout0->addWidget(slider);
    layout->addLayout(layout0);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));

    group = new QButtonGroup(this);
    btn_salt = new QRadioButton(tr("salt"),this);
    btn_salt->setChecked(true);
    btn_pepper = new QRadioButton(tr("pepper"),this);
    btn_Gaussian = new QRadioButton(tr("Gaussian"),this);
    group->addButton(btn_salt, BTN_SALT);
    group->addButton(btn_pepper, BTN_PEPPER);
    group->addButton(btn_Gaussian, BTN_GAUSSIAN);
    hlayout->addWidget(btn_salt);
    hlayout->addWidget(btn_pepper);
    hlayout->addWidget(btn_Gaussian);
    layout->addLayout(hlayout);

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

NoiseDialog::~NoiseDialog()
{
    delete layout, hlayout, group, slider;
    delete btn_salt, btn_pepper, btn_Gaussian;
    delete button, check_preview;
}

void NoiseDialog::accept()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    Mat* src = img->mat;
    Mat* mat_show = new Mat(src->rows, src->cols, CV_8UC1);
    src->copyTo(*mat_show);
    switch(group->checkedId()){
    case BTN_SALT:
        if(!saltNoise(mat_show, slider->value()))return;
        break;
    case BTN_PEPPER:
        if(!pepperNoise(mat_show, slider->value()))return;
        break;
    case BTN_GAUSSIAN:
        break;
    default:return;
    }
    img->updateImg(mat_show);
    QDialog::accept();
}

void NoiseDialog::reject()
{
    QDialog::reject();
}

void NoiseDialog::on_value_changed()
{
    if(!check_preview->isChecked())return;
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    Mat* src = img->mat;
    Mat mat_show(src->rows, src->cols, CV_8UC1);
    src->copyTo(mat_show);
    switch(group->checkedId()){
    case BTN_SALT:
        if(!saltNoise(&mat_show, slider->value()))return;
        break;
    case BTN_PEPPER:
        if(!pepperNoise(&mat_show, slider->value()))return;
        break;
    case BTN_GAUSSIAN:
        break;
    default:return;
    }
    img->showImg(&mat_show);

}
