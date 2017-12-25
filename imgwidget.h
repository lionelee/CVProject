#ifndef IMGWIDGET_H
#define IMGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPaintEvent>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>
#include <vector>
#include "imgprocess.h"
#include "cutmodal.h"

class ImgWidget: public QLabel
{
    Q_OBJECT

public:
    ImgWidget(QWidget *parent = 0);
    ~ImgWidget();

    bool isSaved = true;
    cv::Mat* mat;
    std::vector<cv::Mat*>backup;
    CutModal* cutModal = NULL;

    bool open(std::string srcPath);
    bool save();
    bool saveAs(std::string dstPath);
    void showImg(cv::Mat* src);
    void updateImg(cv::Mat* src);
    void undo();
    void redo();

    void setScaleType(int type);
    void setRotateType(int type);
    void setCut();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    /*functions*/

private:
    /*varivables*/
    std::string filePath;
    QString mouseInfo[6];
    size_t idx = 0;

    int sca_type = BILINEAR;
    int rot_type = BILINEAR;
    bool cut_flag = false;
};

#endif // IMGWIDGET_H
