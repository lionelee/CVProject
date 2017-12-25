#ifndef CUTMODAL_H
#define CUTMODAL_H

#include <QWidget>

class CutModal: public QWidget
{
    Q_OBJECT

public:
    CutModal(QWidget* parent = NULL);
    ~CutModal();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool mouse_down = false;
};

#endif // CUTMODAL_H
