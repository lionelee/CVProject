#include "colorleveldialog.h"
#include "mainwindow.h"
#include "imgwidget.h"

ColorLevelDialog::ColorLevelDialog(QWidget* parent)
    :QDialog(parent)
{
    this->setWindowTitle(tr("Adjust Color Level"));
    layout = new QVBoxLayout(this);

    combobox = new QComboBox(this);
    combobox->addItem(tr("RGB"));
    combobox->addItem(tr("Red"));
    combobox->addItem(tr("Green"));
    combobox->addItem(tr("Blue"));
    layout->addWidget(combobox);

    QHBoxLayout *layout_ib = new QHBoxLayout(this);
    in_black = new QSlider(Qt::Horizontal,this);
    in_black->setRange(0,255);
    in_black->setValue(0);
    label_ib = new QLabel(tr("in black:0"));
    layout_ib->addWidget(label_ib);
    layout_ib->addWidget(in_black);
    layout->addLayout(layout_ib);

    QHBoxLayout *layout_ig = new QHBoxLayout(this);
    in_gray = new QSlider(Qt::Horizontal,this);
    in_gray->setRange(1,999);
    in_gray->setValue(100);
    label_ig = new QLabel(tr("in gray:1"));
    layout_ig->addWidget(label_ig);
    layout_ig->addWidget(in_gray);
    layout->addLayout(layout_ig);

    QHBoxLayout *layout_iw = new QHBoxLayout(this);
    in_white = new QSlider(Qt::Horizontal,this);
    in_white->setRange(0,255);
    in_white->setValue(255);
    label_iw = new QLabel(tr("in white:255"));
    layout_iw->addWidget(label_iw);
    layout_iw->addWidget(in_white);
    layout->addLayout(layout_iw);

    connect(in_black,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    connect(in_gray,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    connect(in_white,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));

    QHBoxLayout *layout_ob = new QHBoxLayout(this);
    out_black = new QSlider(Qt::Horizontal,this);
    out_black->setRange(0,255);
    out_black->setValue(0);
    label_ob = new QLabel(tr("out black:0"));
    layout_ob->addWidget(label_ob);
    layout_ob->addWidget(out_black);
    layout->addLayout(layout_ob);

    QHBoxLayout *layout_ow = new QHBoxLayout(this);
    out_white = new QSlider(Qt::Horizontal,this);
    out_white->setRange(0,255);
    out_white->setValue(255);
    label_ow = new QLabel(tr("out white:255"));
    layout_ow->addWidget(label_ow);
    layout_ow->addWidget(out_white);
    layout->addLayout(layout_ow);

    connect(out_black,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    connect(out_white,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));

    check_preview = new QCheckBox(tr("preview"),this);
    check_preview->setChecked(true);
    layout->addWidget(check_preview);

    QDialogButtonBox *button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(button);
    this->setLayout(layout);

    ImgWidget* img = (ImgWidget*)((MainWindow*)parent)->imgWidget;
    Mat* src = img->mat;
    mat_show = new Mat(src->rows, src->cols, CV_8UC3);
}

ColorLevelDialog::~ColorLevelDialog()
{
    delete layout, combobox, check_preview, button;
    delete in_black, in_gray, in_white;
    delete out_black, out_white;
    delete label_ib, label_ig, label_iw, label_ob, label_ow;
}

void ColorLevelDialog::accept()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    int ib = in_black->value(), iw = in_white->value();
    int tmp = 0;
    if(ib > iw){
        tmp = iw;
        iw = ib;
        ib = tmp;
    }
    int ob = out_black->value(), ow = out_white->value();
    if(ob > ow){
        tmp = ow;
        ow = ob;
        ob = ow;
    }
    double ig = in_gray->value() / 100.0;
    int channel = combobox->currentIndex();
    adjustColorlevel(img->mat, mat_show, channel, ib, ig, iw, ob, ow);
    img->updateImg(mat_show);
    QDialog::accept();
}

void ColorLevelDialog::reject()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    img->showImg(img->mat);
    QDialog::reject();
}

void ColorLevelDialog::on_value_changed()
{
    if(!check_preview->isChecked())return;
    int ib = in_black->value(), iw = in_white->value();
    int tmp = 0;
    if(ib > iw){
        tmp = iw;
        iw = ib;
        ib = tmp;
    }
    int ob = out_black->value(), ow = out_white->value();
    if(ob > ow){
        tmp = ow;
        ow = ob;
        ob = tmp;
    }
    double ig = in_gray->value() / 100.0;
    label_ib->setText(tr("in black:")+QString::fromStdString(num2str(ib)));
    label_ig->setText(tr("in gray:")+QString::fromStdString(double2str(ig)));
    label_iw->setText(tr("in white:")+QString::fromStdString(num2str(iw)));
    label_ob->setText(tr("out black:")+QString::fromStdString(num2str(ob)));
    label_ow->setText(tr("out white:")+QString::fromStdString(num2str(ow)));
    int channel = combobox->currentIndex();
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    if(!adjustColorlevel(img->mat, mat_show, channel, ib, ig, iw, ob, ow))return;
    img->showImg(mat_show);
}
