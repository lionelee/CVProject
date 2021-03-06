#include "binarymorphology.h"
#include "mainwindow.h"
#include "imgwidget.h"
#include <QMessageBox>
#include <QFileDialog>

BinaryMorphology::BinaryMorphology(QWidget *parent, int _type)
    :QDialog(parent),type(_type)
{
    this->setWindowTitle(tr("Binary Morphology"));
    layout = new QVBoxLayout(this);
    grid = new QGridLayout();

    for(int i = 0; i < 16; ++i){
        spin[i] = new QSpinBox(this);
        spin[i]->setRange(-10,10);
        spin[i]->setValue(0);
        grid->addWidget(spin[i], i/4, i%4);
    }
    layout->addLayout(grid);

    text = new QTextEdit(this);
    text->setVisible(false);
    layout->addWidget(text);

    QHBoxLayout* layout1 = new QHBoxLayout(this);
    anchorx = new QSpinBox(this);
    anchorx->setMinimum(0);
    anchorx->setValue(1);
    anchory = new QSpinBox(this);
    anchory->setMinimum(0);
    anchory->setValue(1);
    layout1->addWidget(new QLabel(tr("anchorx:")));
    layout1->addWidget(anchorx);
    layout1->addWidget(new QLabel(tr("anchory:")));
    layout1->addWidget(anchory);
    layout->addLayout(layout1);

    iter = new QSpinBox(this);
    iter->setMinimum(1);
    iter->setValue(1);
    QHBoxLayout* layout2 = new QHBoxLayout(this);
    QLabel* label_iter = new QLabel(tr("iteration:"));
    layout2->addWidget(label_iter);
    layout2->addWidget(iter);
    layout->addLayout(layout2);

    if(type != OPENRBD && type != CLOSERBD){
        iter->setVisible(false);
        label_iter->setVisible(false);
    }

    group = new QButtonGroup(this);
    btn_two = new QRadioButton(tr("two"),this);
    btn_three = new QRadioButton(tr("three"),this);
    btn_four = new QRadioButton(tr("four"),this);
    btn_four->setChecked(true);
    btn_custom = new QRadioButton(tr("custom"),this);
    group->addButton(btn_two, 2);
    group->addButton(btn_three,3);
    group->addButton(btn_four,4);
    group->addButton(btn_custom,5);
    connect(group,SIGNAL(buttonClicked(int)),this,SLOT(on_btn_clicked()));
    layout->addWidget(btn_two);
    layout->addWidget(btn_three);
    layout->addWidget(btn_four);
    layout->addWidget(btn_custom);

    button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(button);
    this->setLayout(layout);
}

BinaryMorphology::~BinaryMorphology()
{
    delete layout, grid, group, button;
    delete btn_two, btn_three, btn_four, btn_custom;
    for(int i =0; i < 16; ++i) delete spin[i];
    delete text, anchorx, anchory, iter;
}

bool BinaryMorphology::getCustomKernel(Mat *kernel)
{
    QString qstr = text->toPlainText();
    if(qstr==NULL || qstr==""){
        QMessageBox::critical(this,tr("error"),tr("nothing found"));
        return false;
    }
    std::string str = qstr.toStdString();
    std::vector<std::vector<char>> vec;
    std::vector<char> row_ele;
    int rows = 0, tmp = 1, cols = 0;
    for(int i = 0; i < str.length(); ++i){
        char c = str[i];
        if(c==','){
            ++tmp;
        }else if(c=='\n'){
            cols = MAX(tmp, cols);
            ++rows;
            vec.push_back(row_ele);
            row_ele.clear();
            tmp = 1;
        }else if(c == '-' ){
            ++i;
            c= - (str[i] -'0');
            row_ele.push_back(c);
        }else{
            row_ele.push_back(c-'0');
        }
    }
    *kernel = Mat(rows, cols, CV_8SC1, Scalar::all(0));
    for(int i = 0; i < rows; ++i){
        char* data = kernel->ptr<char>(i);
        for(int j = 0; j < vec[i].size(); ++j){
            data[j] = vec[i][j];
        }
    }
    return true;
}

void BinaryMorphology::operation(Mat* kernel)
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    Mat* src = img->mat;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    Mat* marker;
    Mat* tmp;
    int ax = anchorx->value(), ay = anchory->value();
    if(ax >= kernel->cols || ay >= kernel->rows) return;
    switch (type) {
    case EROSION:
        erosion(src, dst, kernel, ax, ay);
        break;
    case DILATION:
        dilation(src, dst, kernel, ax, ay);
        break;
    case OPEN:
        tmp = new Mat(src->rows, src->cols, CV_8UC1);
        erosion(src, tmp, kernel, ax, ay);
        dilation(tmp, dst, kernel, ax, ay);
        delete tmp;
        break;
    case CLOSE:
        tmp = new Mat(src->rows, src->cols, CV_8UC1);
        dilation(src, tmp, kernel, ax, ay);
        erosion(tmp, dst, kernel, ax, ay);
        delete tmp;
        break;
    case HITMISS:
        hitMiss(src, dst, kernel, ax, ay);
        break;
    case THINNING:
        thinning(src, dst, kernel, ax, ay);
        break;
    case THICKING:
        thickening(src, dst, kernel, ax, ay);
        break;
    case SKELETON:
        skeleton(src, dst, kernel, ax, ay);
        break;
    case SKERECON:
        skeletonReconstruct(src, dst, kernel, ax, ay);
        break;
    case EROSRBD:
        marker = openFile();
        if(marker==NULL)return;
        erosionRebuild(marker, src, dst, kernel, ax, ay);
        break;
    case DILARBD:
        marker = openFile();
        if(marker==NULL)return;
        dilationRebuild(marker, src, dst, kernel, ax, ay);
        break;
    case OPENRBD:
        openRebuild(src, dst, kernel, iter->value(), ax, ay);
        break;
    case CLOSERBD:
        closeRebuild(src, dst, kernel, iter->value(), ax, ay);
        break;
    default: return;
    }
    img->updateImg(dst);
}

Mat* BinaryMorphology::openFile()
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

void BinaryMorphology::accept()
{
    int id = group->checkedId();
    Mat kernel;
    switch (id) {
    case 2:
        kernel = (Mat_<char>(2,2)<<spin[0]->value(),spin[1]->value(),spin[4]->value(),spin[5]->value());
        break;
    case 3:
        kernel = (Mat_<char>(3,3)<<spin[0]->value(),spin[1]->value(),spin[2]->value(),
                                    spin[4]->value(),spin[5]->value(),spin[6]->value(),
                                    spin[8]->value(),spin[9]->value(),spin[10]->value());
        break;
    case 4:
        kernel = Mat(3,3,CV_8SC1);
        for(int i =0; i < 4; ++i){
            kernel.data[i] = spin[i]->value();
        }
        break;
    case 5:
        if(!getCustomKernel(&kernel)){
            QDialog::accept();
            return;
        }
        break;
    default:
        QDialog::accept();
        return;
    }
    QDialog::accept();
    operation(&kernel);
}

void BinaryMorphology::reject()
{
    QDialog::reject();
}

void BinaryMorphology::on_btn_clicked()
{
    int id = group->checkedId();
    switch (id) {
    case 2:
        text->setVisible(false);
        iter->setVisible(false);
        for(int i = 0; i < 16; ++i){
            if(i<2 || i==4 || i==5){
                spin[i]->setVisible(true);
            }else{
                spin[i]->setVisible(false);
            }
        }
        break;
    case 3:
        text->setVisible(false);
        for(int i = 0; i < 16; ++i){
            if(i==3 || i==7 || i>10){
                spin[i]->setVisible(false);
            }else{
                spin[i]->setVisible(true);
            }
        }
        break;
    case 4:
        text->setVisible(false);
        for(int i = 0; i < 16; ++i){
            spin[i]->setVisible(true);
        }
        break;
    case 5:
        for(int i = 0; i < 16; ++i){
            spin[i]->setVisible(false);
        }
        text->setVisible(true);
        break;
    default: return;
    }
}
