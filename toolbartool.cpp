#include "toolbartool.h"
#include "mainwindow.h"
#include "imgwidget.h"
#include <QBoxLayout>
#include <QMenu>
#include <QLabel>

QString BtnQSS("QPushButton{padding:4px;background:transparent;}"
               "QPushButton:focus{padding:4px;background:lightblue;border:1px solid blue;}");

ToolbarTool::ToolbarTool(const QString &title, QWidget *parent)
    :QToolBar(title, parent)
{
    this->setMovable(true);
    this->setOrientation(Qt::Vertical);
    group = new QButtonGroup(this);
    group->setExclusive(true);

    btn_cur = new QPushButton(this);
    btn_cur->setIcon(QIcon(":/res/tools/cursor.png"));
    btn_cur->setIconSize(QSize(24,24));
    btn_cur->setStatusTip(tr("cursor"));
    btn_cur->setStyleSheet(BtnQSS);
    btn_cur->setFocus();
    group->addButton(btn_cur, BTN_CURSOR);
    connect(btn_cur,SIGNAL(clicked(bool)),this, SLOT(on_btn_cur_clicked()));

    btn_alg = new QPushButton(this);
    btn_alg->setIcon(QIcon(":/res/tools/algebra.png"));
    btn_alg->setIconSize(QSize(24,24));
    btn_alg->setStatusTip(tr("algebra operation"));
    btn_alg->setStyleSheet(BtnQSS);
    group->addButton(btn_alg, BTN_ALGEBRA);
    connect(btn_alg,SIGNAL(clicked(bool)),this, SLOT(on_btn_alg_clicked()));

    btn_rot = new QPushButton(this);
    btn_rot->setIcon(QIcon(":/res/tools/rotate.png"));
    btn_rot->setIconSize(QSize(24,24));
    btn_rot->setStatusTip(tr("rotate image"));
    btn_rot->setStyleSheet(BtnQSS);
    group->addButton(btn_rot, BTN_ROTATE);
    connect(btn_rot,SIGNAL(clicked(bool)),this, SLOT(on_btn_rot_clicked()));

    btn_sca = new QPushButton(this);
    btn_sca->setIcon(QIcon(":/res/tools/scale.png"));
    btn_sca->setIconSize(QSize(24,24));
    btn_sca->setStatusTip(tr("scale type"));
    btn_sca->setStyleSheet(BtnQSS);
    QMenu* menu_sca = new QMenu(this);
    sca_bilinear = new QAction(tr("bilinear"),this);
    sca_bilinear->setStatusTip(tr("bilinear"));
    connect(sca_bilinear,SIGNAL(triggered(bool)),this,SLOT(on_sca_bilinear_triggered()));
    sca_nearest = new QAction(tr("nearest"),this);
    sca_nearest->setStatusTip(tr("nearest"));
    connect(sca_nearest,SIGNAL(triggered(bool)),this,SLOT(on_sca_nearest_triggered()));
    menu_sca->addAction(sca_bilinear);
    menu_sca->addAction(sca_nearest);
    btn_sca->setMenu(menu_sca);
    group->addButton(btn_sca, BTN_SCALE);

    btn_cut = new QPushButton(this);
    btn_cut->setIcon(QIcon(":/res/tools/cut.png"));
    btn_cut->setIconSize(QSize(24,24));
    btn_cut->setStatusTip(tr("cut image"));
    btn_cut->setStyleSheet(BtnQSS);
    group->addButton(btn_cut, BTN_CUT);
    connect(btn_cut,SIGNAL(clicked(bool)),this, SLOT(on_btn_cut_clicked()));

    btn_con = new QPushButton(this);
    btn_con->setIcon(QIcon(":/res/tools/contrast.png"));
    btn_con->setIconSize(QSize(24,24));
    btn_con->setStatusTip(tr("contrast gradient adjustment"));
    btn_con->setStyleSheet(BtnQSS);
    group->addButton(btn_con, BTN_CONTRAST);
    connect(btn_con,SIGNAL(clicked(bool)),this, SLOT(on_btn_con_clicked()));

    btn_filter = new QPushButton(this);
    btn_filter->setIcon(QIcon(":/res/tools/filter.png"));
    btn_filter->setIconSize(QSize(24,24));
    btn_filter->setStatusTip(tr("smooth filter"));
    btn_filter->setStyleSheet(BtnQSS);
    QMenu* menu_filter = new QMenu(this);
    filter_normal = new QAction(tr("smooth filter"),this);
    connect(filter_normal,SIGNAL(triggered(bool)),this,SLOT(on_filter_normal_triggered()));
    filter_custom = new QAction(tr("custom filter"),this);
    connect(filter_custom,SIGNAL(triggered(bool)),this,SLOT(on_filter_custom_triggered()));
    menu_filter->addAction(filter_normal);
    menu_filter->addAction(filter_custom);
    btn_filter->setMenu(menu_filter);
    group->addButton(btn_filter, BTN_FILTER);

    btn_noise = new QPushButton(this);
    btn_noise->setIcon(QIcon(":/res/tools/noise.png"));
    btn_noise->setIconSize(QSize(24,24));
    btn_noise->setStatusTip(tr("noise"));
    btn_noise->setStyleSheet(BtnQSS);
    group->addButton(btn_noise, BTN_NOISE);
    connect(btn_noise,SIGNAL(clicked(bool)),this, SLOT(on_btn_noise_clicked()));

    this->addWidget(btn_cur);
    this->addWidget(btn_alg);
    this->addWidget(btn_rot);
    this->addWidget(btn_sca);
    this->addWidget(btn_cut);
    this->addWidget(btn_con);
    this->addWidget(btn_filter);
    this->addWidget(btn_noise);
}

ToolbarTool::~ToolbarTool()
{
    delete group;
    delete sca_bilinear, sca_nearest, filter_normal, filter_custom;
    delete btn_cur, btn_alg, btn_rot, btn_cut, btn_con, btn_filter, btn_noise;
}

void ToolbarTool::on_btn_cur_clicked()
{
    ((MainWindow*)parent())->toolbarOpt->setCursorOpt();
}

void ToolbarTool::on_btn_alg_clicked()
{
    ((MainWindow*)parent())->toolbarOpt->setAlgebraOpt();
}

void ToolbarTool::on_btn_rot_clicked()
{
    ((MainWindow*)parent())->toolbarOpt->setRotateOpt();
}

void ToolbarTool::on_btn_cut_clicked()
{
    ((MainWindow*)parent())->toolbarOpt->setCutOpt();
}

void ToolbarTool::on_btn_con_clicked()
{
    ((MainWindow*)parent())->adjustContrast();
}

void ToolbarTool::on_btn_noise_clicked()
{
    ((MainWindow*)parent())->addNoise();
}

void ToolbarTool::on_sca_bilinear_triggered()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    if(img==NULL)return;
    img->setScaleType(BILINEAR);
}

void ToolbarTool::on_sca_nearest_triggered()
{
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    if(img==NULL)return;
    img->setScaleType(NEAREST);
}

void ToolbarTool::on_filter_normal_triggered()
{
    ((MainWindow*)parent())->smoothFilter();
}

void ToolbarTool::on_filter_custom_triggered()
{

}
