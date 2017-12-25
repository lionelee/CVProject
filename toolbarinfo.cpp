#include "toolbarinfo.h"
#include "mainwindow.h"
#include "imgwidget.h"
#include "imgprocess.h"
#include <QFormLayout>
#include <QScrollArea>
#include <QMessageBox>

ToolbarInfo::ToolbarInfo(const QString& title, QWidget* parent)
    :QToolBar(title, parent)
{
    this->setMovable(true);
    this->setOrientation(Qt::Vertical);
    tabs = new QTabWidget(this);

    QWidget *minfo = new QWidget();
    QFormLayout *mlayout = new QFormLayout();
    label_posx = new QLabel();
    label_posy = new QLabel();
    label_red = new QLabel();
    label_blue = new QLabel();
    label_green = new QLabel();
    label_gray = new QLabel();
    label_gray->setVisible(false);
    label_posx->setText(tr("posx: "));
    label_posy->setText(tr("posy: "));
    label_red->setText(tr("red: "));
    label_green->setText(tr("green: "));
    label_blue->setText(tr("blue: "));


    mlayout->addWidget(label_posx);
    mlayout->addWidget(label_posy);
    mlayout->addWidget(label_red);
    mlayout->addWidget(label_blue);
    mlayout->addWidget(label_green);
    mlayout->addWidget(label_gray);
    minfo->setLayout(mlayout);
    tabs->addTab(minfo,tr("Info"));

//    QWidget *cinfo = new QWidget();
//    QFormLayout *clayout = new QFormLayout();
//    group = new QButtonGroup();
//    chl_rgb = new QRadioButton(tr("channel RGB"));
//    chl_rgb->setChecked(true);
//    chl_red = new QRadioButton(tr("channel RED"));
//    chl_green = new QRadioButton(tr("channel GREEN"));
//    chl_blue = new QRadioButton(tr("channel BLUE"));

//    group->addButton(chl_rgb);
//    group->setId(chl_rgb, CHANNEL_RGB);
//    group->addButton(chl_red);
//    group->setId(chl_red,CHANNEL_R);
//    group->addButton(chl_green);
//    group->setId(chl_green,CHANNEL_G);
//    group->addButton(chl_blue);
//    group->setId(chl_blue,CHANNEL_B);

//    clayout->addWidget(chl_rgb);
//    clayout->addWidget(chl_red);
//    clayout->addWidget(chl_green);
//    clayout->addWidget(chl_blue);
//    cinfo->setLayout(clayout);
//    tabs->addTab(cinfo,tr("Channel"));
    this->addWidget(tabs);
}

ToolbarInfo::~ToolbarInfo()
{
    delete tabs;
    delete label_red, label_blue, label_green;
    delete label_posx, label_posy;
}

void ToolbarInfo::updateMouseInfo(QString(&info)[6])
{

    if(info[5]==""){
        label_posx->setText(tr("posx: ")+info[0]);
        label_posy->setText(tr("posy: ")+info[1]);
        label_red->setVisible(true);
        label_green->setVisible(true);
        label_blue->setVisible(true);
        label_gray->setVisible(false);
        label_red->setText(tr("red: ")+info[2]);
        label_green->setText(tr("green: ")+info[3]);
        label_blue->setText(tr("blue: ")+info[4]);
    }else{
        label_posx->setText(tr("posx: ")+info[0]);
        label_posy->setText(tr("posy: ")+info[1]);
        label_red->setVisible(false);
        label_green->setVisible(false);
        label_blue->setVisible(false);
        label_gray->setVisible(true);
        label_gray->setText(tr("gray: ")+info[5]);
    }
}

/*private slots functions*/


