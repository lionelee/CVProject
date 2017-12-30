#include "toolbaropt.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imgwidget.h"
#include <QBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

ToolbarOpt::ToolbarOpt(const QString &title, QWidget *parent)
    :QToolBar(title, parent)
{
    this->setMovable(true);
    this->setOrientation(Qt::Horizontal);

    cur_opt.push_back(((MainWindow*)parent)->ui->action_New);
    cur_opt.push_back(((MainWindow*)parent)->ui->action_Open);
    cur_opt.push_back(((MainWindow*)parent)->ui->action_Close);
    cur_opt.push_back(((MainWindow*)parent)->ui->action_Save);
    cur_opt.push_back(((MainWindow*)parent)->ui->action_Save_As);
    this->addAction(cur_opt[0]);
    this->addAction(cur_opt[1]);
    this->addAction(cur_opt[2]);
    this->addAction(cur_opt[3]);
    this->addAction(cur_opt[4]);
    idx = CUR_OPT;

    act_add = new QAction(QIcon(":/res/tools/add.png"),tr("add operation"),this);
    act_add->setToolTip(tr("add operation"));
    act_add->setStatusTip(tr("add operation"));
    connect(act_add, SIGNAL(triggered(bool)),this,SLOT(on_act_add_triggered()));
    act_diff = new QAction(QIcon(":/res/tools/diff.png"),tr("minus operation"),this);
    act_diff->setToolTip(tr("minus operation"));
    act_diff->setStatusTip(tr("minus operation"));
    connect(act_diff, SIGNAL(triggered(bool)),this,SLOT(on_act_diff_triggered()));
    act_mul = new QAction(QIcon(":/res/tools/mul.png"),tr("multiply operation"),this);
    act_mul->setToolTip(tr("multiply operation"));
    act_mul->setStatusTip(tr("minus operation"));
    connect(act_mul, SIGNAL(triggered(bool)),this,SLOT(on_act_mul_triggered()));
    alg_opt.push_back(act_add);
    alg_opt.push_back(act_diff);
    alg_opt.push_back(act_mul);
}

ToolbarOpt::~ToolbarOpt()
{
    delete act_add, act_diff, act_mul;
}


void ToolbarOpt::setCursorOpt()
{
    removeLast();
    idx = CUR_OPT;
    for(int i = 0; i < cur_opt.size(); ++i){
        this->addAction(cur_opt[i]);
    }
}

void ToolbarOpt::setAlgebraOpt()
{
    removeLast();
    idx = ALG_OPT;
    for(int i = 0; i < alg_opt.size(); ++i){
        this->addAction(alg_opt[i]);
    }
}

void ToolbarOpt::setCutOpt()
{

}


/*private slots functions*/
void ToolbarOpt::on_act_add_triggered()
{

    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    Mat* src = img->mat;
    if(img == NULL || src == NULL)return;
    Mat* dst = openFile();
    if(dst == NULL) return;
    if(addMat(src, dst)){
        img->updateImg(dst);
    }
}

void ToolbarOpt::on_act_diff_triggered()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    Mat* src = img->mat;
    if(img == NULL || src == NULL)return;
    Mat* dst = openFile();
    if(dst == NULL) return;
    if(diffMat(src, dst)){
        img->updateImg(dst);
    }
}

void ToolbarOpt::on_act_mul_triggered()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    Mat* src = img->mat;
    if(img == NULL || src == NULL)return;
    Mat* dst = openFile();
    if(dst == NULL) return;
    if(mulMat(src, dst)){
        img->updateImg(dst);
    }
}

/*private functions*/
void ToolbarOpt::removeLast()
{
    switch (idx) {
    case CUR_OPT:
        for(int i = 0; i < cur_opt.size(); ++i){
            this->removeAction(cur_opt[i]);
        }
        break;
    case ALG_OPT:
        for(int i = 0; i < alg_opt.size(); ++i){
            this->removeAction(alg_opt[i]);
        }
        break;
    default: break;
    }
}

bool ToolbarOpt::openFile(Mat *dst)
{
    QString filter = tr("JPEG(*.jpg;*.jpeg;*.jpe);;PNG(*.png;*.pns);;RAW(*.raw);;BITMAP(*.bmp);;All Format(*.*)");
    QString promt = QString::fromLocal8Bit("打开图片");
    QString filePath = QFileDialog::getOpenFileName(this, promt, "", filter);

    if(filePath==NULL) return false;
    bool flag = true;
    Mat tmp = imread(filePath.toStdString(),-1);
    if(!tmp.data){
        flag = false;
    }else{
        int cols = tmp.cols;
        int rows = tmp.rows;
        switch(tmp.channels()){
        case 1:
            dst = new Mat(rows, cols, CV_8UC1);
            cpMatGray(&tmp, dst);
            break;
        case 3:
            dst = new Mat(rows, cols, CV_8UC3);
            if(!cvtBGR2RGB(&tmp, dst))return false;
            break;
        case 4:
            dst = new Mat(rows, cols, CV_8UC4);
            break;
        default:
            flag = false;
            break;
        }
    }

    if(!flag){
        QString promt = QString::fromLocal8Bit("错误");
        QString msg = QString::fromLocal8Bit("无法打开图片 ");
        QMessageBox::critical(this,promt, msg+filePath,QMessageBox::Ok, QMessageBox::Ok);
    }
    return flag;
}


Mat* ToolbarOpt::openFile()
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
        switch(tmp.channels()){
        case 1:
            dst = new Mat(rows, cols, CV_8UC1);
            cpMatGray(&tmp, dst);
            break;
        case 3:
            dst = new Mat(rows, cols, CV_8UC3);
            if(!cvtBGR2RGB(&tmp, dst))return false;
            break;
        case 4:
            dst = new Mat(rows, cols, CV_8UC4);
            break;
        default:
            flag = false;
            break;
        }
    }

    if(!flag){
        QString promt = QString::fromLocal8Bit("错误");
        QString msg = QString::fromLocal8Bit("无法打开图片 ");
        QMessageBox::critical(this,promt, msg+filePath,QMessageBox::Ok, QMessageBox::Ok);
        return NULL;
    }
    return dst;
}
