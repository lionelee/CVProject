#include "histogramwidget.h"
#include <QPainter>

HistogramWidget::HistogramWidget(QWidget* parent, int* _hist )
    :QLabel(parent)
{
    this->setMinimumWidth(100);
    this->setMinimumHeight(80);
    hist = _hist;
}

HistogramWidget::~HistogramWidget()
{
    if(hist != NULL) delete hist;
}

void HistogramWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    int width=this->width();
    int height=this->height();
    p.setBrush(QBrush(QColor(0,0,0)));
    p.drawRect(0,0,width,height);
    int max = 0;
    for(int i = 0; i < 256; ++i){
        if(hist[i] > max) max = hist[i];
    }

    double xstep = double(width) / 258;
    double ystep = double(height) / max;

    p.setPen(QColor(255,255,255));
    for (int i = 0; i < 256; ++i){
        int x = (i+1)*xstep;
        p.drawLine(QPointF(x, height),QPointF(x, height-hist[i]*ystep));
    }
}

void HistogramWidget::updateHist(int* _hist )
{
    hist = _hist;
    update();
}
