#include "scaledialog.h"
#include "mainwindow.h"
#include "imgwidget.h"

ScaleDialog::ScaleDialog(QWidget* parent)
    :QDialog(parent)
{
    this->setWindowTitle(tr("Scale Image"));
    layout = new QVBoxLayout(this);
    combobox = new QComboBox(this);
    combobox->addItem(tr("Bilinear"));
    combobox->addItem(tr("Nearest"));
    layout->addWidget(combobox);

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(1, 1000);
    slider->setValue(100);
    label = new QLabel(tr("ratio:100"),this);
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

ScaleDialog::~ScaleDialog()
{
    delete layout, layout1, button;
    delete combobox, slider, label, check_preview;
}

void ScaleDialog::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    Mat* src = img->mat;
    double ratio = slider->value()/100.0;
    int rows = src->rows*ratio, cols = src->cols*ratio;
    Mat* dst;
    if(src->channels()==1){
        dst = new Mat(rows, cols, CV_8UC1);
        scaleC1(src, dst, combobox->currentIndex());
    }else if(src->channels()==3){
        dst = new Mat(rows, cols, CV_8UC3);
        scaleC3(src, dst, combobox->currentIndex());
    }else{
        QDialog::accept();
        return;
    }
    img->updateImg(dst);
    std::string str = num2str(dst->rows)+"x"+num2str(dst->cols);
    ((MainWindow*)parent())->toolbarInfo->updateSize(str);
    QDialog::accept();
}

void ScaleDialog::reject()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    img->showImg(img->mat);
    QDialog::reject();
}

void ScaleDialog::on_value_changed()
{
    int a = slider->value();
    label->setText(tr("Ratio:")+QString::fromStdString(num2str(a)));
    if(!check_preview->isChecked())return;
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    Mat* src = img->mat;
    double ratio = slider->value()/100.0;
    int rows = src->rows*ratio, cols = src->cols*ratio;
    Mat* dst;
    if(src->channels()==1){
        dst = new Mat(rows, cols, CV_8UC1);
        scaleC1(src, dst, combobox->currentIndex());
    }else if(src->channels()==3){
        dst = new Mat(rows, cols, CV_8UC3);
        scaleC3(src, dst, combobox->currentIndex());
    }else return;
    img->showImg(dst);
    delete dst;
}

