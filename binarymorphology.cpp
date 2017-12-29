#include "binarymorphology.h"
#include "mainwindow.h"
#include "imgwidget.h"

BinaryMorphology::BinaryMorphology(QWidget *parent, int _type)
    :QDialog(parent),type(_type)
{
    layout = new QVBoxLayout(this);
    grid = new QGridLayout(4,4);

    for(int i = 0; i < 16; ++i){
        spin[i] = new QSpinBox(this);
        spin[i]->setRange(0,1);
        grid->addWidget(spin[i], i/4, i%4);
    }

    group = new QButtonGroup(this);
    btn_two = new QRadioButton(tr("two"),this);
    btn_three = new QRadioButton(tr("three"),this);
    btn_four = new QRadioButton(tr("four"),this);
    btn_custom = new QRadioButton(tr("custom"),this);
    group->addButton(btn_two, 2);
    group->addButton(btn_three,3);
    group->addButton(btn_four,4);
    group->addButton(btn_custom,5);
    connect(group,SIGNAL(buttonClicked(int)),this,SLOT(on_btn_clicked()));
    layout->addWidget(btn_two);
    layout->addWidget(btn_three);
    layout->addWidget(btn_four);
    layout->addWidget(btn_custom);

    button = new QDialogButtonBox(this);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(button);
    this->setLayout(layout);
}

BinaryMorphology::~BinaryMorphology()
{
    delete layout, grid, group, button;
    delete btn_two, btn_three, btn_four, btn_custom;
    for(int i =0; i < 16; ++i) delete spin[i];
}

void BinaryMorphology::accept()
{
    QDialog::accept();
}

void BinaryMorphology::reject()
{
    ImgWidget* img = (ImgWidget*)(((MainWindow*)parent())->scrollArea->widget());
    img->showImg(img->mat);
    QDialog::reject();
}

void BinaryMorphology::on_btn_clicked()
{
    int id = group->checkedId();
    switch(id){

    }
}
