#ifndef IMGWIDGET_H
#define IMGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPaintEvent>
#include <QMouseEvent>
#include <vector>
#include "imgprocess.h"

class ImgWidget: public QLabel
{
    Q_OBJECT

public:
    ImgWidget(QWidget *parent = 0);
    ~ImgWidget();

    bool isSaved = true;
    cv::Mat* mat;
    std::vector<cv::Mat*>backup;

    bool open(std::string srcPath);
    bool save();
    bool saveAs(std::string dstPath);
    void showImg(cv::Mat* src);
    void updateImg(cv::Mat* src);
    void undo();
    void redo();

    void setScaleType(int type);
    void setCut();
    void unsetCut();
    void updateCut(int x, int y, int w, int h);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent);
    void wheelEvent(QWheelEvent *event);

    /*functions*/

private:
    /*varivables*/
    std::string filePath;
    QString mouseInfo[6];
    size_t idx = 0;

    int sca_type = BILINEAR;
    bool cut_flag = false;
    bool mouse_down = false;

    /*cut modal dialog*/
    QRect cut_rect;
};

#endif // IMGWIDGET_H
