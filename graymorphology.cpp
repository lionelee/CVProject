#include "graymorphology.h"
#include "imgwidget.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

GrayMorphology::GrayMorphology(QWidget* parent, int _type)
    :QDialog(parent), type(_type)
{
    layout = new QVBoxLayout(this);
    QHBoxLayout *layout_row = new QHBoxLayout(this);
    label_row = new QLabel(tr("rows:"));
    spin_row = new QSpinBox(this);
    spin_row->setRange(1, 100);
    spin_row->setValue(3);
    layout_row->addWidget(label_row);
    layout_row->addWidget(spin_row);
    layout->addLayout(layout_row);

    QHBoxLayout *layout_col = new QHBoxLayout(this);
    label_col = new QLabel(tr("col:"));
    spin_col = new QSpinBox(this);
    spin_col->setRange(1, 100);
    spin_col->setValue(3);
    layout_col->addWidget(label_col);
    layout_col->addWidget(spin_col);
    layout->addLayout(layout_col);

    if(type == GOPENRBD || type == GCLOSERBD){
        QHBoxLayout *layout_iter = new QHBoxLayout(this);
        label_iter = new QLabel(tr("iteration"));
        spin_iter = new QSpinBox(this);
        spin_iter->setRange(1, 50);
        spin_iter->setValue(10);
        layout_iter->addWidget(label_iter);
        layout_iter->addWidget(spin_iter);
        layout->addLayout(layout_iter);
    }else{
        label_iter = NULL;
        spin_iter = NULL;
    }

    button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(button);
    this->setLayout(layout);
}

GrayMorphology::~GrayMorphology()
{
    delete layout, button;
    delete label_row, label_col, spin_row, spin_col;
    if(label_iter != NULL)delete label_iter;
    if(spin_iter != NULL) delete spin_iter;
}

Mat* GrayMorphology::openFile()
{
    QString filter = tr("JPEG(*.jpg;*.jpeg;*.jpe);;PNG(*.png;*.pns);;RAW(*.raw);;BITMAP(*.bmp);;All Format(*.*)");
    QString promt = QString::fromLocal8Bit("打开图片");
    QString filePath = QFileDialog::getOpenFileName(this, promt, "", filter);

    if(filePath==NULL) return NULL;
    bool flag = true;
    Mat tmp = imread(filePath.toStdString(),-1);
    Mat* dst;
    if(!tmp.data){
        flag = false;
    }else{
        int cols = tmp.cols;
        int rows = tmp.rows;
        if(tmp.channels()==1){
            dst = new Mat(rows, cols, CV_8UC1);
            if(!dst->data){
                flag = false;
            }else{
                cpMatGray(&tmp, dst);
            }
        }else{
            flag = false;
            return NULL;
        }
    }

    if(!flag){
        QString promt = QString::fromLocal8Bit("错误");
        QString msg = QString::fromLocal8Bit("无法打开图片 ");
        QMessageBox::critical(this,promt, msg+filePath,QMessageBox::Ok, QMessageBox::Ok);
    }
    return dst;
}

void GrayMorphology::accept()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    Mat* src = img->mat; Mat *tmp;
    int rows = spin_row->value(), cols = spin_col->value();
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    QDialog::accept();
    switch (type) {
    case GEROSION:
        grayErosion(src, dst, rows, cols);
        break;
    case GDILATION:
        grayDilation(src, dst, rows, cols);
        break;
    case GOPEN:
        tmp = new Mat(src->rows, src->cols, CV_8UC1);
        grayErosion(src, tmp, rows, cols);
        grayDilation(tmp, dst, rows, cols);
        delete tmp;
        break;
    case GCLOSE:
        tmp = new Mat(src->rows, src->cols, CV_8UC1);
        grayDilation(src, tmp, rows, cols);
        grayErosion(tmp, dst, rows, cols);
        delete tmp;
        break;
    case GEROSRBD:
        tmp = openFile();
        if(tmp==NULL)return;
        grayErosionRebuild(tmp, src, dst, rows, cols);
        delete tmp;
        break;
    case GDILARBD:
        tmp = openFile();
        if(tmp==NULL)return;
        grayDilationRebuild(tmp, src, dst, rows, cols);
        delete tmp;
        break;
    case GOPENRBD:
        grayOpenRebuild(src, dst, rows, cols, spin_iter->value());
        break;
    case GCLOSERBD:
        grayCloseRebuild(src, dst, rows, cols, spin_iter->value());
        break;
    default: return;
    }
    img->updateImg(dst);
}

void GrayMorphology::reject()
{
    QDialog::reject();
}
