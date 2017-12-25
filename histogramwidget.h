#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QLabel>

class HistogramWidget: public QLabel
{
    Q_OBJECT

public:
    explicit HistogramWidget(QWidget* parent = 0, int* _hist = NULL);
    ~HistogramWidget();

    void updateHist(int* _hist);
    void paintEvent(QPaintEvent* event);

private:
    int* hist;
};


#endif // HISTOGRAMWIDGET_H
