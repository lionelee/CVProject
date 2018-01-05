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
        if(imwrite(dstPath, *mat)){
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
    this->resize(src->cols,src->rows);
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

void ImgWidget::setCut()
{
    cut_flag = true;
    int width = this->width(), height = this->height();
    cut_rect = QRect(3*width/8,3*height/8,width/4,height/4);
}

void ImgWidget::unsetCut()
{
    cut_flag = false;
}

void ImgWidget::updateCut(int x, int y, int w, int h)
{
    cut_rect = QRect(x, y, w, h);
    this->update();
}


/*protected functions*/
void ImgWidget::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    if(cut_flag){
        QPainterPath painterPath;
        QPainterPath p;
        p.addRect(0,0,this->width(),this->height());
        painterPath.addRect(cut_rect);
        QPainterPath drawPath =p.subtracted(painterPath);

        QPainter paint(this);
        paint.setOpacity(0.8);
        paint.fillPath(drawPath, QBrush(Qt::black));
    }
}

void ImgWidget::mousePressEvent(QMouseEvent *event)
{
    mouse_down = true;
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
        int delta = event->delta();
        Mat* mat_scale;
        if(delta > 0){     //up
            if(mat->channels()==1){
                mat_scale = new Mat(mat->rows*1.1, mat->cols*1.1, CV_8UC1);
                scaleC1(mat, mat_scale, sca_type);
            }else if(mat->channels()==3){
                mat_scale = new Mat(mat->rows*1.1, mat->cols*1.1, CV_8UC3);
                scaleC3(mat, mat_scale, sca_type);
            }
        } else if(delta < 0){    //down
            if(mat->channels()==1){
                mat_scale = new Mat(mat->rows*0.9, mat->cols*0.9, CV_8UC1);
                scaleC1(mat, mat_scale, sca_type);
            }else if(mat->channels()==3){
                mat_scale = new Mat(mat->rows*0.9, mat->cols*0.9, CV_8UC3);
                scaleC3(mat, mat_scale, sca_type);
            }
        }else{
            return;
        }
        updateImg(mat_scale);
        event->accept();
        std::string str = num2str(mat->rows)+"x"+num2str(mat->cols);
        w->toolbarInfo->updateSize(str);
        return;

    }
}
