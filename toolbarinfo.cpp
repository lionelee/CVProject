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
    label_posx = new QLabel(tr("posx: "), this);
    label_posy = new QLabel(tr("posy: "), this);
    label_red = new QLabel(tr("red: "), this);
    label_blue = new QLabel(tr("green: "), this);
    label_green = new QLabel(tr("blue: "), this);
    label_gray = new QLabel();
    label_gray->setVisible(false);
    mlayout->addWidget(label_posx);
    mlayout->addWidget(label_posy);
    mlayout->addWidget(label_red);
    mlayout->addWidget(label_blue);
    mlayout->addWidget(label_green);
    mlayout->addWidget(label_gray);
    minfo->setLayout(mlayout);
    tabs->addTab(minfo,tr("Mouse info"));

    QWidget *cinfo = new QWidget();
    QFormLayout *clayout = new QFormLayout();
    label_path = new QLabel(this);
    label_size = new QLabel(this);
    label_channel = new QLabel(this);
    clayout->addWidget(label_path);
    clayout->addWidget(label_size);
    clayout->addWidget(label_channel);
    cinfo->setLayout(clayout);
    tabs->addTab(cinfo,tr("Image info"));
    this->addWidget(tabs);
}

ToolbarInfo::~ToolbarInfo()
{
    delete tabs;
    delete label_red, label_blue, label_green;
    delete label_posx, label_posy;
    delete label_path, label_size, label_channel;
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

void ToolbarInfo::updateImageInfo(QString &path, std::string size, std::string channel)
{
    label_path->setText(tr("path:")+path);
    label_path->setWordWrap(true);
    label_path->adjustSize();
    label_size->setText(tr("size:")+QString::fromStdString(size));
    label_channel->setText(tr("channel:")+QString::fromStdString(channel));
}

void ToolbarInfo::updatePath(QString &path)
{
    label_path->setText(tr("path:")+path);
    label_path->setWordWrap(true);
    label_path->adjustSize();
}

void ToolbarInfo::updateSize(std::string size)
{
    label_size->setText(tr("size:")+QString::fromStdString(size));
}

void ToolbarInfo::updateChannel(std::string channel)
{
    label_channel->setText(tr("channel:")+QString::fromStdString(channel));
}
