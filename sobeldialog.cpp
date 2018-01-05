#include "sobeldialog.h"
#include "mainwindow.h"
#include "imgwidget.h"


QString checkedStyleSheet("border-style: solid; background-color: LightGray;");
QString uncheckedStyleSheet("border-style: solid; background-color: transparent;");

SobelDialog::SobelDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle(tr("Sobel Edge Detection"));
    layout = new QVBoxLayout(this);
    hlayout = new QHBoxLayout(this);
    group = new QButtonGroup(this);
    group->setExclusive(true);

    btn_ho = new QToolButton(this);
    btn_ho->setIcon(QIcon(":/res/operator/sobel_ho.png"));
    btn_ho->setStyleSheet(uncheckedStyleSheet);
    btn_ho->setIconSize(QSize(72,72));
    btn_ho->setCheckable(true);
    btn_ve = new QToolButton(this);
    btn_ve->setIcon(QIcon(":/res/operator/sobel_ve.png"));
    btn_ve->setStyleSheet(uncheckedStyleSheet);
    btn_ve->setIconSize(QSize(72,72));
    btn_ve->setCheckable(true);
    btn_nw = new QToolButton(this);
    btn_nw->setIcon(QIcon(":/res/operator/sobel_nw.png"));
    btn_nw->setStyleSheet(uncheckedStyleSheet);
    btn_nw->setIconSize(QSize(72,72));
    btn_nw->setCheckable(true);
    btn_ne = new QToolButton(this);
    btn_ne->setIcon(QIcon(":/res/operator/sobel_ne.png"));
    btn_ne->setStyleSheet(uncheckedStyleSheet);
    btn_ne->setIconSize(QSize(72,72));
    btn_ne->setCheckable(true);

    group->addButton(btn_ho, BTN_HORIZONTAL);
    group->addButton(btn_ve, BTN_VERTICAL);
    group->addButton(btn_nw, BTN_NORTHWEST);
    group->addButton(btn_ne, BTN_NORTHEAST);
    connect(group,SIGNAL(buttonToggled(int,bool)),this,SLOT(on_btn_toggled(int,bool)));
    hlayout->addWidget(btn_ho);
    hlayout->addWidget(btn_ve);
    hlayout->addWidget(btn_nw);
    hlayout->addWidget(btn_ne);
    layout->addLayout(hlayout);

    button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(button);
    this->setLayout(layout);

}

SobelDialog::~SobelDialog()
{
    delete layout, hlayout;
    delete button, group;
    delete btn_ho, btn_ve, btn_nw, btn_ne;
}

void SobelDialog::accept()
{
    int type = group->checkedId();
    if(type == -1){
        QDialog::accept();
        return;
    }
    ImgWidget* img = ((MainWindow*)parent())->imgWidget;
    if(img == NULL)return;
    Mat* src = img->mat;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    SobelDetect(src, dst, type);
    img->updateImg(dst);
    QDialog::accept();
}

void SobelDialog::reject()
{
    QDialog::reject();
}

void SobelDialog::on_btn_toggled(int id, bool stat)
{
    if(stat == false){
        group->button(id)->setStyleSheet(uncheckedStyleSheet);
    }else{
        group->checkedButton()->setStyleSheet(checkedStyleSheet);
    }
}
