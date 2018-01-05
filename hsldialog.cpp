#include "hsldialog.h"
#include "mainwindow.h"
#include "imgwidget.h"
#include <QDialogButtonBox>
#include <QBoxLayout>

HSLDialog::HSLDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Adjust HSL"));
    QVBoxLayout* layout_hsl = new QVBoxLayout();

    QHBoxLayout* hlayout_hue = new QHBoxLayout();
    label_hue = new QLabel(tr("hue:0"));
    slider_hue = new QSlider(Qt::Horizontal);
    slider_hue->setMaximum(240);
    slider_hue->setMinimum(0);
    hlayout_hue->addWidget(label_hue);
    hlayout_hue->addWidget(slider_hue);
    connect(slider_hue, SIGNAL(valueChanged(int)), this, SLOT(on_value_changed(int)));

    QHBoxLayout* hlayout_sat = new QHBoxLayout();
    slider_sat = new QSlider(Qt::Horizontal);
    slider_sat->setMaximum(240);
    slider_sat->setMinimum(0);
    label_sat = new QLabel(tr("sat:0"));
    hlayout_sat->addWidget(label_sat);
    hlayout_sat->addWidget(slider_sat);
    connect(slider_sat, SIGNAL(valueChanged(int)), this, SLOT(on_value_changed(int)));

    QHBoxLayout* hlayout_lig = new QHBoxLayout();
    slider_lig = new QSlider(Qt::Horizontal);
    slider_lig->setMaximum(240);
    slider_lig->setMinimum(0);
    label_lig = new QLabel(tr("lig:0"));
    hlayout_lig->addWidget(label_lig);
    hlayout_lig->addWidget(slider_lig);
    connect(slider_lig, SIGNAL(valueChanged(int)), this, SLOT(on_value_changed(int)));

    layout_hsl->addLayout(hlayout_hue);
    layout_hsl->addLayout(hlayout_sat);
    layout_hsl->addLayout(hlayout_lig);

    check_preview = new QCheckBox(tr("preview"),this);
    check_preview->setChecked(true);
    layout_hsl->addWidget(check_preview);

    QDialogButtonBox *button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout_hsl->addWidget(button);
    this->setLayout(layout_hsl);

    ImgWidget* img = (ImgWidget*)(((MainWindow*)this->parent())->scrollArea->widget());
    Mat* src = img->mat;
    mat_hsl = new Mat(src->rows, src->cols, CV_64FC3);
    cvtRGB2HSL(src, mat_hsl);
}

HSLDialog::~HSLDialog()
{
    delete label_hue, label_sat, label_lig;
    delete slider_hue, slider_sat, slider_lig;
    delete check_preview;
}

void HSLDialog::accept()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    Mat* src = img->mat;
    Mat mat_tmp(src->rows, src->cols, CV_64FC3);
    adjusthsl(mat_hsl, &mat_tmp,slider_hue->value(), slider_sat->value(), slider_lig->value());
    Mat* new_mat = new Mat(src->rows, src->cols, CV_8UC3);
    cvtHSL2RGB(&mat_tmp, new_mat);
    img->updateImg(new_mat);
    QDialog::accept();
}

void HSLDialog::reject()
{
    ImgWidget* img = (ImgWidget*)((MainWindow*)parent())->imgWidget;
    img->showImg(img->mat);
    QDialog::reject();
}

void HSLDialog::on_value_changed(int)
{
    if(check_preview->isChecked()){
        ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
        Mat mat_tmp(mat_hsl->rows, mat_hsl->cols, CV_64FC3);
        int hue = slider_hue->value(), sat = slider_sat->value(), lig = slider_lig->value();
        label_hue->setText(tr("hue:")+QString::fromStdString(num2str(hue)));
        label_sat->setText(tr("sat:")+QString::fromStdString(num2str(sat)));
        label_lig->setText(tr("lig:")+QString::fromStdString(num2str(lig)));
        adjusthsl(mat_hsl, &mat_tmp,hue, sat, lig);
        Mat mat_show(mat_hsl->rows, mat_hsl->cols, CV_8UC3);
        cvtHSL2RGB(&mat_tmp, &mat_show);
        img->showImg(&mat_show);
    }
}
