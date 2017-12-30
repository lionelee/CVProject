#include "rotatedialog.h"
#include "mainwindow.h"
#include "imgwidget.h"

RotateDialog::RotateDialog(QWidget* parent)
    :QDialog(parent)
{
    layout = new QVBoxLayout(this);
    combobox = new QComboBox(this);
    combobox->addItem(tr("Bilinear"));
    combobox->addItem(tr("Nearest"));
    layout->addWidget(combobox);

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(-180, 180);
    slider->setValue(0);
    label = new QLabel(tr("Angle:0"),this);
    layout1 = new QHBoxLayout(this);
    layout1->addWidget(label);
    layout1->addWidget(slider);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    layout->addLayout(layout1);

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

RotateDialog::~RotateDialog()
{
    delete layout, layout1, button;
    delete combobox, slider, label, check_preview;
}

void RotateDialog::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    Mat* src = img->mat;
    int r = src->rows, c = src->cols;
    double angle = slider->value()*0.01745, sa = fabs(sin(angle)), ca = fabs(cos(angle));
    int rows = ceil(r*ca+c*sa), cols = ceil(r*sa+c*ca);
    Mat* dst;
    if(src->channels()==1){
        dst = new Mat(rows, cols, CV_8UC1);
        rotateC1(src, dst, angle, combobox->currentIndex());
    }else if(src->channels()==3){
        dst = new Mat(rows, cols, CV_8UC3);
        rotateC3(src, dst, angle, combobox->currentIndex());
    }else{
        QDialog::accept();
        return;
    }
    img->updateImg(dst);
    std::string str = num2str(dst->rows)+"x"+num2str(dst->cols);
    ((MainWindow*)parent())->toolbarInfo->updateSize(str);
    QDialog::accept();
}

void RotateDialog::reject()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    img->showImg(img->mat);
    QDialog::reject();
}

void RotateDialog::on_value_changed()
{
    int a = slider->value();
    label->setText(tr("Angle:")+QString::fromStdString(num2str(a)));
    if(!check_preview->isChecked())return;
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    Mat* src = img->mat;
    int r = src->rows, c = src->cols;
    double angle = slider->value()*0.01745, sa = fabs(sin(angle)), ca = fabs(cos(angle));
    int rows = ceil(r*ca+c*sa), cols = ceil(r*sa+c*ca);
    Mat* dst;
    if(src->channels()==1){
        dst = new Mat(rows, cols, CV_8UC1);
        rotateC1(src, dst, angle, combobox->currentIndex());
    }else if(src->channels()==3){
        dst = new Mat(rows, cols, CV_8UC3);
        rotateC3(src, dst, angle, combobox->currentIndex());
    }else return;
    img->showImg(dst);
    delete dst;
}


