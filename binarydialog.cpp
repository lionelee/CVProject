#include "binarydialog.h"
#include "mainwindow.h"
#include "imgwidget.h"
#include "imgprocess.h"
#include <QLabel>
#include <QBoxLayout>
#include <QMessageBox>

BinaryDialog::BinaryDialog(QWidget *parent)
    :QDialog(parent)
{
    QVBoxLayout* layout_binary = new QVBoxLayout();

    int* hist = new int[256];
    for(int i = 0; i < 256; ++i){
        hist[i] = 0;
    }
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent)->imgWidget);
    getHistogram(img->mat, hist);
    histWidget = new HistogramWidget(this, hist);
    layout_binary->addWidget(histWidget);

    QHBoxLayout* hlayout_min = new QHBoxLayout();
    QLabel* label_min = new QLabel(tr("min"));
    slider_min = new QSlider(Qt::Horizontal);
    slider_min->setMaximum(255);
    slider_min->setMinimum(0);
    slider_min->setTickPosition(QSlider::TicksBelow);
    hlayout_min->addWidget(label_min);
    hlayout_min->addWidget(slider_min);
    connect(slider_min, SIGNAL(valueChanged(int)), this, SLOT(on_value_changed()));

    QHBoxLayout* hlayout_max = new QHBoxLayout();
    QLabel* label_max = new QLabel(tr("max"));
    slider_max = new QSlider(Qt::Horizontal);
    slider_max->setMaximum(255);
    slider_max->setMinimum(0);
    slider_max->setTickPosition(QSlider::TicksBelow);
    hlayout_max->addWidget(label_max);
    hlayout_max->addWidget(slider_max);
    connect(slider_max, SIGNAL(valueChanged(int)), this, SLOT(on_value_changed()));
    layout_binary->addLayout(hlayout_min);
    layout_binary->addLayout(hlayout_max);

    check_preview = new QCheckBox(tr("preview"),this);
    check_preview->setChecked(true);
    layout_binary->addWidget(check_preview);

    button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout_binary->addWidget(button);
    this->setLayout(layout_binary);
}

BinaryDialog::~BinaryDialog()
{
    delete slider_min, slider_max;
    delete histWidget, button;
}

void BinaryDialog::accept()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    int min = slider_min->value(), max = slider_max->value();
    if(min > max){
        max += min;
        min = max - min;
        max = max - min;
    }
    Mat* src = img->mat;
    Mat* mat_binary = new Mat(src->rows, src->cols, CV_8UC1);
    if(!handsonBinary(src, mat_binary, min, max))return;
    img->updateImg(mat_binary);
    QDialog::accept();
}

void BinaryDialog::reject()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    img->showImg(img->mat);
    QDialog::reject();
}

void BinaryDialog::on_value_changed()
{
    if(!check_preview->isChecked())return;
    int min = slider_min->value(), max = slider_max->value();
    if(min > max){
        int tmp = max;
        max = min;
        min = tmp;
    }
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    Mat* src = img->mat;
    Mat mat_binary(src->rows, src->cols, CV_8UC1);
    if(!handsonBinary(src, &mat_binary, min, max))return;
    img->showImg(&mat_binary);
}
