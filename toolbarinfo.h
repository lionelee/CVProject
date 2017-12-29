#ifndef TOOLBARINFO_H
#define TOOLBARINFO_H

#include <QWidget>
#include <QToolBar>
#include <QTabWidget>
#include <QLabel>

class ToolbarInfo: public QToolBar
{
    Q_OBJECT

public:
    ToolbarInfo(const QString& title, QWidget* parent = 0);
    ~ToolbarInfo();

    void updateMouseInfo(QString(&info)[6]);
    void updateImageInfo(QString& path, std::string size, std::string channel);

private:
    /*widgets*/
    QTabWidget *tabs;
    QLabel *label_posx, *label_posy;
    QLabel *label_red, *label_blue, *label_green, *label_gray;
    QLabel *label_path, *label_size, *label_channel;

};

#endif // TOOLBARINFO_H
