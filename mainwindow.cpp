#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imgwidget.h"

#include <QTabWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>

/*public fcuntions*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/mipmap/lens_32.png"));
    ui->centralWidget->setMouseTracking(true);


    tabWidget = new QTabWidget();
    tabWidget->setTabsClosable(true);
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(on_tab_Close_triggered(int)));

    QGridLayout *layout = new QGridLayout();
    layout->setMargin(0);
    layout->addWidget(tabWidget);
    ui->centralWidget->setLayout(layout);

    QTabWidget *attr_tab = new QTabWidget();
    QLabel *label = new QLabel();
    attr_tab->addTab(label,tr("pic"));
    ui->toolBar_attributes->addWidget(attr_tab);

    label_posx = new QLabel();
    label_posy = new QLabel();
    label_red = new QLabel();
    label_blue = new QLabel();
    label_green = new QLabel();

    label_posx->setText(tr("posx: "));
    label_posy->setText(tr("posy: "));
    label_red->setText(tr("red: "));
    label_blue->setText(tr("blue: "));
    label_green->setText(tr("green: "));

    QWidget *widget_details = new QWidget();
    QVBoxLayout *vLayout0 = new QVBoxLayout();
    vLayout0->addWidget(label_posx);
    vLayout0->addWidget(label_posy);
    vLayout0->addWidget(label_red);
    vLayout0->addWidget(label_blue);
    vLayout0->addWidget(label_green);
    widget_details->setLayout(vLayout0);
    ui->toolBar_details->addWidget(widget_details);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tabWidget;
    delete label_posx, label_posy;
    delete label_red, label_blue, label_green;
}

/*protected functions*/
//bool MainWindow::eventFilter(QObject *watched, QEvent *event)
//{
//    ImgWidget *obj = (ImgWidget*)tabWidget->currentWidget();
//    if(watched != obj){
//        label_posx->setText(tr("posx: "));
//        label_posy->setText(tr("posy: "));
//        label_red->setText(tr("red: "));
//        label_blue->setText(tr("blue: "));
//        label_green->setText(tr("green: "));
//        return true;
//    }
//    return false;
//}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for(int i = total; i >= 0; --i)
        close(i);
    exit(0);
}

/*private slots functions*/
void MainWindow::on_action_New_triggered()
{
}


void MainWindow::on_action_Open_triggered()
{
    QString filter = tr("JPEG(*.jpg;*.jpeg;*.jpe);;PNG(*.png;*.pns);;RAW(*.raw);;BITMAP(*.bmp);;All Format(*.*)");
    QString promt = QString::fromLocal8Bit("打开图片");
    QString filePath = QFileDialog::getOpenFileName(this, promt, "", filter);

    if(filePath==NULL){
        return;
    }
    for(int i = 0; i < total; ++i){
        if(fileOpened[i] == filePath){
            tabWidget->setCurrentIndex(i);
            return;
        }
    }

    QChar sep('/');
    QString title = filePath.split(sep).back();
    ImgWidget *imgWidget = new ImgWidget(this);
    if(!imgWidget->open(filePath.toStdString())){
        QString promt = QString::fromLocal8Bit("错误");
        QString msg = QString::fromLocal8Bit("无法打开图片 ");
        QMessageBox::critical(this,promt, msg+filePath,QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(imgWidget);
    scrollArea->setAlignment(Qt::AlignCenter);

    tabWidget->addTab(scrollArea,title);
    tabWidget->setCurrentIndex(total);

    fileOpened.push_back(filePath);
    isSaved.push_back(true);
    ++total;
}

void MainWindow::on_action_Close_triggered()
{
    int idx = tabWidget->currentIndex();
    close(idx);
}

void MainWindow::on_action_Save_triggered()
{
    int idx = tabWidget->currentIndex();
    if(idx < 0 || idx > total-1)
        return;
    save(idx);
}

void MainWindow::on_action_Save_As_triggered()
{
    int idx = tabWidget->currentIndex();
    if(idx < 0 || idx > total-1)
        return;
    saveAs(idx);
}

void MainWindow::on_action_Exit_triggered()
{
    for(int i = total-1; i >= 0; --i)
        close(i);
    exit(0);
}

void MainWindow::on_action_Undo_triggered()
{

}

void MainWindow::on_action_Redo_triggered()
{

}

void MainWindow::on_action_Help_triggered()
{

}

void MainWindow::on_action_About_triggered()
{

}

void MainWindow::on_tab_Close_triggered(int idx){
    close(idx);
}

/*private functions*/
void MainWindow::close(int idx)
{
    if(idx < 0 || idx > total-1)
        return;
    if(!isSaved[idx]){
        QChar sep('/');
        QString fileName = fileOpened[idx].split(sep).back();
        QString promt = QString::fromLocal8Bit("警告");
        QString msg0 = QString::fromLocal8Bit("是否在关闭前保存对 ");
        QString msg1 = QString::fromLocal8Bit(" 的更改?");
        QMessageBox::StandardButton btn = QMessageBox::warning(this,promt,msg0+fileName+msg1,
                                          QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);
        if(btn ==  QMessageBox::Yes){
            save(idx);
        }
        else if(btn == QMessageBox::Cancel){
            return;
        }
    }
    std::vector<QString>::iterator it1 = fileOpened.begin()+idx;
    fileOpened.erase(it1);
    std::vector<bool>::iterator it2 = isSaved.begin()+idx;
    isSaved.erase(it2);
    tabWidget->removeTab(idx);
    --total;
}

void MainWindow::save(int idx)
{
    QString filePath = fileOpened[idx];
    isSaved[idx] = true;
}

void MainWindow::saveAs(int idx)
{
    QString filter = tr("JPEG(*.jpg;*.jpeg;*.jpe);;PNG(*.png;*.pns);;RAW(*.raw);;BITMAP(*.bmp)");
    QString promt = QString::fromLocal8Bit("图片另存为");
    QString fileName = QFileDialog::getSaveFileName(NULL,promt,"",filter);
    isSaved[idx] = true;
}


