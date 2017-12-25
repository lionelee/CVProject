#ifndef TOOLBARINFO_H
#define TOOLBARINFO_H

#include <QWidget>
#include <QToolBar>
#include <QTabWidget>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>

class ToolbarInfo: public QToolBar
{
    Q_OBJECT

public:
    ToolbarInfo(const QString& title, QWidget* parent = 0);
    ~ToolbarInfo();

    void updateMouseInfo(QString(&info)[6]);

private slots:

private:
    /*widgets*/
    QTabWidget *tabs;
    QLabel *label_posx, *label_posy;
    QLabel *label_red, *label_blue, *label_green, *label_gray;

    QButtonGroup *group;
    QRadioButton *chl_rgb, *chl_red, *chl_blue, *chl_green;

};

#endif // TOOLBARINFO_H
