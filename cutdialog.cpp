#include "cutdialog.h"
#include "mainwindow.h"
#include "imgwidget.h"

CutDialog::CutDialog(QWidget *parent)
    :QDialog(parent)
{
    layout = new QVBoxLayout(this);

    label_px = new QLabel(tr("posx:"),this);
    px = new QSpinBox(this);
    px->setMaximum(0);
    QHBoxLayout* layout0 = new QHBoxLayout(this);
    layout0->addWidget(label_px);
    layout0->addWidget(py);
    layout->addLayout(layout0);

    label_py = new QLabel(tr("posy:"),this);
    py = new QSpinBox(this);
    py->setMaximum(0);
    QHBoxLayout* layout1 = new QHBoxLayout(this);
    layout1->addWidget(label_py);
    layout1->addWidget(py);
    layout->addLayout(layout1);

    label_cw = new QLabel(tr("width:"),this);
    cw = new QSpinBox(this);
    cw->setMaximum(0);
    QHBoxLayout* layout2 = new QHBoxLayout(this);
    layout2->addWidget(label_cw);
    layout2->addWidget(cw);
    layout->addLayout(layout2);

    label_ch = new QLabel(tr("hight:"),this);
    ch = new QSpinBox(this);
    ch->setMaximum(0);
    QHBoxLayout* layout3 = new QHBoxLayout(this);
    layout3->addWidget(label_ch);
    layout3->addWidget(ch);
    layout->addLayout(layout3);

    button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(button);
    this->setLayout(layout);
}

CutDialog::~CutDialog()
{
    delete layout, button;
    delete label_px, label_py, label_cw, label_ch;
    delete px, py, cw, ch;
}

void CutDialog::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    Mat* src = img->mat;
    int rangex = src->cols, rangey = src->rows;
    int x = px->value(), y = py->value();
    int w = cw->value(), h = ch->value();
    if(x > rangex-1 || y > rangey-1 || w > rangex || h > rangey){
        QDialog::accept();
        return;
    }
    Mat* dst;
    if(src->channels()==1){
        dst = new Mat(h, w, CV_8UC1);
        cutC1(src, dst, x, y);
    }else if(src->channels()==1){
        dst = new Mat(h, w, CV_8UC3);
        cutC3(src, dst, x, y);
    }else{
        QDialog::accept();
        return;
    }
    img->updateImg(dst);
    QDialog::accept();
}

void CutDialog::reject()
{
    QDialog::reject();
}
