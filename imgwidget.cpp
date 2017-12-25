#include "imgwidget.h"
#include "mainwindow.h"
#include <QPainter>
#include <QMessageBox>

extern MainWindow *w;

using namespace std;

ImgWidget::ImgWidget(QWidget *parent)
    :QLabel(parent)
{
    setMouseTracking(true);
}

ImgWidget::~ImgWidget(){}

/*public functions*/
bool ImgWidget::open(std::string srcPath)
{
    filePath = srcPath;
    Mat tmp = imread(srcPath,-1);
    if(!tmp.data){
        return false;
    }
    int cols = tmp.cols;
    int rows = tmp.rows;
    switch(tmp.channels()){
    case 1:
        mat = new Mat(rows, cols, CV_8UC1);
        cpMatGray(&tmp, mat);
        break;
    case 3:
        mat = new Mat(rows, cols, CV_8UC3);
        if(!cvtBGR2RGB(&tmp, mat))return false;
        break;
    case 4:
        mat = new Mat(rows, cols, CV_8UC4);
        break;
    default:return false;
    }
    backup.push_back(mat);
    showImg(mat);
    return true;
}

bool ImgWidget::save()
{
    if(!isSaved){
        isSaved = true;
        if(mat->channels()==3){
            Mat tmp(mat->rows, mat->cols, CV_8UC3);
            if(!cvtRGB2BGR(mat, &tmp))return false;
            return imwrite(filePath, tmp);
        }else if(mat->channels()==1){
            return imwrite(filePath, *mat);
        }else{
            return false;
        }
    } else {
        return true;
    }
}

bool ImgWidget::saveAs(std::string dstPath)
{
    isSaved = true;
    if(mat->channels()==3){
        Mat tmp(mat->rows, mat->cols, CV_8UC3);
        if(!cvtRGB2BGR(mat, &tmp))return false;
        if(imwrite(dstPath, tmp)){
            filePath = dstPath;
            return true;
        } else{
            return false;
        }
    }else if(mat->channels()==1){
        if(imwrite(filePath, *mat)){
            filePath = dstPath;
            return true;
        } else{
            return false;
        }
    }else{
        return false;
    }
}

void ImgWidget::showImg(Mat* src)
{
    QImage img;
    switch (src->channels()) {
    case 1:
        img = QImage((const uchar*)src->data,src->cols, src->rows,
                            src->cols, QImage::Format_Indexed8);
        break;
    case 3:
    case 4:
        img = QImage((const uchar*)src->data,src->cols, src->rows,
                            src->cols * 3,QImage::Format_RGB888);
        break;
    default: return;
    }
    QPixmap pixmap = QPixmap::fromImage(img);
    this->setPixmap(pixmap);
}

void ImgWidget::updateImg(Mat* src)
{
    isSaved = false;
    mat = src;
    ++idx;
    while(backup.size() > idx){
        if(backup.back() != NULL)
            backup.pop_back();
    }
    backup.push_back(mat);
    showImg(mat);
}

void ImgWidget::undo()
{
    --idx;
    if(((long long)idx)<0){
        idx = 0;
        return;
    }
    mat = backup[idx];
    showImg(mat);
}

void ImgWidget::redo()
{
    ++idx;
    if(idx >= backup.size()){
        idx = backup.size();
        return;
    }
    mat = backup[idx];
    showImg(mat);
}

void ImgWidget::setScaleType(int type)
{
    sca_type = type;
}

void ImgWidget::setRotateType(int type)
{
    rot_type = type;
}

void ImgWidget::setCut()
{
    cut_flag = true;
    cutModal = new CutModal(this);
}


/*protected functions*/

void ImgWidget::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    if(cut_flag && cutModal != NULL){
        QPainterPath painterPath;
        QPainterPath p;
        p.addRect(x(),y(),rect().width(),rect().height());
        painterPath.addRect(cutModal->geometry());
        QPainterPath drawPath =p.subtracted(painterPath);

        QPainter paint(this);
        paint.setOpacity(0.7);
        paint.fillPath(drawPath,QBrush(Qt::black));
    }
}

void ImgWidget::mouseMoveEvent(QMouseEvent *event)
{
    int posx = event->x();
    int posy = event->y();
    if(posx <0 || posx >= mat->cols || posy < 0 ||posy >= mat->rows)
        return;
    int red, green, blue, gray;
    switch(mat->channels()){
    case 1:
        gray = mat->at<uchar>(posy, posx);
        mouseInfo[2] = "";
        mouseInfo[3] = "";
        mouseInfo[4] = "";
        mouseInfo[5] = QString::fromStdString(num2str(gray));
        break;
    case 3:
    case 4:
        red = mat->at<Vec3b>(posy, posx)[0];
        green = mat->at<Vec3b>(posy, posx)[1];
        blue = mat->at<Vec3b>(posy, posx)[2];
        mouseInfo[2] = QString::fromStdString(num2str(red));
        mouseInfo[3] = QString::fromStdString(num2str(blue));
        mouseInfo[4] = QString::fromStdString(num2str(green));
        mouseInfo[5] = "";
        break;
    default:
        return;
    }
    mouseInfo[0] = QString::fromStdString(num2str(posx));
    mouseInfo[1] = QString::fromStdString(num2str(posy));
    w->updateMouseInfo(mouseInfo);

}

void ImgWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier){
        event->accept();
        if(event->delta() > 0){     //up

        } else {    //down

        }
    }
}
