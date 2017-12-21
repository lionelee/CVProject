#include "imgwidget.h"
#include "mainwindow.h"
#include <QPainter>
#include <QMessageBox>

#include <opencv.hpp>
#include <strstream>

extern MainWindow* w;

std::string num2str(int num){
    std::stringstream ss;
    ss<<num;
    std::string s;
    ss>>s;
    return s;
}

ImgWidget::ImgWidget(QWidget *parent)
    :QLabel(parent)
{
    setMouseTracking(true);
}

ImgWidget::~ImgWidget()
{

}

/*public functions*/
void ImgWidget::mouseMoveEvent(QMouseEvent *event)
{
    int posx = event->x();
    int posy = event->y();
    if(posx <0 || posx >= cols || posy < 0 ||posy >= rows)
        return;
    int blue = mat_show.at<cv::Vec3b>(posx,posy)[0];
    int green = mat_show.at<cv::Vec3b>(posx,posy)[0];
    int red = mat_show.at<cv::Vec3b>(posx,posy)[0];
    QString x = QString::fromStdString(num2str(posx));
    QString y = QString::fromStdString(num2str(posy));
    QString r = QString::fromStdString(num2str(red));
    QString b = QString::fromStdString(num2str(blue));
    QString g = QString::fromStdString(num2str(green));

    w->label_posx->setText(tr("posx: ")+x);
    w->label_posy->setText(tr("posy: ")+y);
    w->label_red->setText(tr("red: ")+r);
    w->label_blue->setText(tr("blue: ")+b);
    w->label_green->setText(tr("green: ")+g);
}

bool ImgWidget::open(std::string srcPath)
{
    filePath = srcPath;
    mat_show = cv::imread(srcPath);
    if(!mat_show.data){
        return false;
    }
    cols = mat_show.cols;
    rows = mat_show.rows;
    showImg();
    this->resize(img_show.size());
    return true;
}


/*private functions*/
void ImgWidget::showImg()
{
    mat2QImage();
    QPixmap pixmap = QPixmap::fromImage(img_show);
    this->setPixmap(pixmap);
}

void ImgWidget::mat2QImage()
{
    if(mat_show.channels()==3){
        cv::Mat rgb = mat_show;
//        cv::cvtColor(mat,rgb, cv::COLOR_BGR2RGB);
        img_show = QImage((const unsigned char*)(rgb.data),
                    rgb.cols, rgb.rows,
                    rgb.cols*rgb.channels(),
                    QImage::Format_RGB888);
    } else {
        img_show = QImage((const unsigned char*)(mat_show.data),
                    cols, rows,
                    cols*mat_show.channels(),
                    QImage::Format_RGB888);
    }
}
