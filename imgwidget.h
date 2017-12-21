#ifndef IMGWIDGET_H
#define IMGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPaintEvent>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>

class ImgWidget: public QLabel
{
    Q_OBJECT

public:
    ImgWidget(QWidget *parent = 0);
    ~ImgWidget();

    bool open(std::string srcPath);
    bool close();
    bool save();
    bool saveAs(std::string dstPath);

protected:
    void mouseMoveEvent(QMouseEvent *event);

private:
    /*variables*/
    int cols;
    int rows;
    QImage img_show;
    cv::Mat mat_show;
    cv::Mat backup[3];
    std::string filePath;

    /*functions*/
    void showImg();
    void mat2QImage();
};

#endif // IMGWIDGET_H
