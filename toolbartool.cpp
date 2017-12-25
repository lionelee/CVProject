#include "toolbartool.h"
#include "mainwindow.h"
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
    btn_cur->setStyleSheet(BtnQSS);
    btn_cur->setFocus();
    group->addButton(btn_cur, BTN_CURSOR);
    connect(btn_cur,SIGNAL(clicked(bool)),this, SLOT(on_btn_cur_clicked()));

    btn_alg = new QPushButton(this);
    btn_alg->setIcon(QIcon(":/res/tools/algebra.png"));
    btn_alg->setIconSize(QSize(24,24));
    btn_alg->setStyleSheet(BtnQSS);
    group->addButton(btn_alg, BTN_ALGEBRA);
    connect(btn_alg,SIGNAL(clicked(bool)),this, SLOT(on_btn_alg_clicked()));

    btn_rot = new QPushButton(this);
    btn_rot->setIcon(QIcon(":/res/tools/rotate.png"));
    btn_rot->setIconSize(QSize(24,24));
    btn_rot->setStyleSheet(BtnQSS);
    group->addButton(btn_rot, BTN_ROTATE);
    connect(btn_rot,SIGNAL(clicked(bool)),this, SLOT(on_btn_rot_clicked()));

    btn_sca = new QPushButton(this);
    btn_sca->setIcon(QIcon(":/res/tools/scale.png"));
    btn_sca->setIconSize(QSize(24,24));
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
    btn_cut->setStyleSheet(BtnQSS);
    group->addButton(btn_cut, BTN_CUT);
    connect(btn_cut,SIGNAL(clicked(bool)),this, SLOT(on_btn_cut_clicked()));

    this->addWidget(btn_cur);
    this->addWidget(btn_alg);
    this->addWidget(btn_rot);
    this->addWidget(btn_sca);
    this->addWidget(btn_cut);
}

ToolbarTool::~ToolbarTool()
{
    delete group;
    delete sca_bilinear, sca_nearest;
    delete btn_cur, btn_alg, btn_rot, btn_cut;
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

void ToolbarTool::on_sca_bilinear_triggered()
{
    ((MainWindow*)parent())->imgWidget->setScaleType(BILINEAR);
}

void ToolbarTool::on_sca_nearest_triggered()
{
    ((MainWindow*)parent())->imgWidget->setScaleType(NEAREST);
}
