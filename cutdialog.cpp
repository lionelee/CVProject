#include "cutdialog.h"
#include "mainwindow.h"
#include "imgwidget.h"

CutDialog::CutDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle(tr("Cut Image"));
    ImgWidget* img = ((MainWindow*)parent)->imgWidget;
    img->setCut();
    int rows = img->mat->rows, cols = img->mat->cols;
    int width = img->width(), height = img->height();
    layout = new QVBoxLayout(this);
    label_px = new QLabel(tr("posx:")+QString::fromStdString(num2str(3*width/8)),this);
    px = new QSlider(Qt::Horizontal,this);
    px->setRange(0, cols-1);
    px->setValue(3*width/8);
    connect(px,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    QHBoxLayout* layout0 = new QHBoxLayout(this);
    layout0->addWidget(label_px);
    layout0->addWidget(px);
    layout->addLayout(layout0);

    label_py = new QLabel(tr("posy:")+QString::fromStdString(num2str(3*height/8)),this);
    py = new QSlider(Qt::Horizontal,this);
    py->setRange(0, rows-1);
    py->setValue(3*height/8);
    connect(py,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    QHBoxLayout* layout1 = new QHBoxLayout(this);
    layout1->addWidget(label_py);
    layout1->addWidget(py);
    layout->addLayout(layout1);

    label_cw = new QLabel(tr("width:")+QString::fromStdString(num2str(width/4)),this);
    cw = new QSlider(Qt::Horizontal,this);
    cw->setRange(1, cols);
    cw->setValue(width/4);
    connect(cw,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    QHBoxLayout* layout2 = new QHBoxLayout(this);
    layout2->addWidget(label_cw);
    layout2->addWidget(cw);
    layout->addLayout(layout2);

    label_ch = new QLabel(tr("hight:")+QString::fromStdString(num2str(height/4)),this);
    ch = new QSlider(Qt::Horizontal,this);
    ch->setRange(1, rows);
    ch->setValue(height/4);
    connect(ch,SIGNAL(valueChanged(int)),this,SLOT(on_value_changed()));
    QHBoxLayout* layout3 = new QHBoxLayout(this);
    layout3->addWidget(label_ch);
    layout3->addWidget(ch);
    layout->addLayout(layout3);

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

CutDialog::~CutDialog()
{
    delete layout, button, check_preview;
    delete label_px, label_py, label_cw, label_ch;
    delete px, py, cw, ch;
}

void CutDialog::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    img->unsetCut();
    Mat* src = img->mat;
    int rangex = src->cols, rangey = src->rows;
    int x = px->value(), y = py->value();
    int w = cw->value(), h = ch->value();
    if(x > rangex-1 || y > rangey-1 || w > rangex || h > rangey){
        QDialog::accept();
        return;
    }
    w = MIN(w, src->rows-x), h = MIN(h, src->cols-y);
    Mat* dst;
    if(src->channels()==1){
        dst = new Mat(h, w, CV_8UC1);
        cutC1(src, dst, x, y);
    }else if(src->channels()==3){
        dst = new Mat(h, w, CV_8UC3);
        cutC3(src, dst, x, y);
    }else{
        QDialog::accept();
        return;
    }
    img->updateImg(dst);
    std::string str = num2str(dst->rows)+"x"+num2str(dst->cols);
    ((MainWindow*)parent())->toolbarInfo->updateSize(str);
    QDialog::accept();
}

void CutDialog::reject()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    img->unsetCut();
    img->showImg(img->mat);
    QDialog::reject();
}

void CutDialog::on_value_changed()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    int x = px->value(), y = py->value();
    int w = cw->value(), h = ch->value();
    w = MIN(w, img->mat->rows-x), h = MIN(h, img->mat->cols-y);
    label_px->setText(tr("posx:")+QString::fromStdString(num2str(x)));
    label_py->setText(tr("posy:")+QString::fromStdString(num2str(y)));
    label_cw->setText(tr("width:")+QString::fromStdString(num2str(w)));
    label_ch->setText(tr("height:")+QString::fromStdString(num2str(h)));
    if(!check_preview->isChecked())return;
    img->updateCut(x, y, w, h);
}
