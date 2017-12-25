#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hsldialog.h"
#include "binarydialog.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/mipmap/lens_32.png"));
    ui->centralWidget->setMouseTracking(true);

    toolbarOpt = new ToolbarOpt(tr("ToolbarOpt"), this);
    this->addToolBar(Qt::TopToolBarArea, toolbarOpt);

    toolbarInfo = new ToolbarInfo(tr("ToolbarInfo"), this);
    this->addToolBar(Qt::RightToolBarArea, toolbarInfo);

    toolbarTool = new ToolbarTool(tr("ToolbarTool"), this);
    this->addToolBar(Qt::LeftToolBarArea, toolbarTool);

    scrollArea = new QScrollArea();
    scrollArea->setAlignment(Qt::AlignCenter);
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(0);
    layout->addWidget(scrollArea);
    ui->centralWidget->setLayout(layout);


    label = new QLabel(this);
    statusBar()->addPermanentWidget(label);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete scrollArea;
}

/*public functions*/
void MainWindow::updateMouseInfo(QString(&info)[6])
{
    toolbarInfo->updateMouseInfo(info);
}
//voi MainWindow::initToolbarAttr(){
//    QTabWidget tabs = new QTabWidget();
//    tabs->setTabsClosable(true);
//    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(on_tab_Close_triggered(int)));

//    QGridLayout *layout = new QGridLayout();

//    QTabWidget *attr_tab = new QTabWidget();
//    QLabel *label = new QLabel();
//    attr_tab->addTab(label,tr("pic"));
//    ui->toolBar_attributes->addWidget(attr_tab);
//}


/*protected functions*/
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    ImgWidget *obj = (ImgWidget*)(scrollArea->widget());
    if(watched != obj){
        return true;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent*)
{
    on_action_Close_triggered();
    exit(0);
}

/*private slots functions*/
void MainWindow::on_action_New_triggered()
{
}


void MainWindow::on_action_Open_triggered()
{
    if(imgWidget != NULL)return;
    QString filter = tr("JPEG(*.jpg;*.jpeg;*.jpe);;PNG(*.png;*.pns);;RAW(*.raw);;BITMAP(*.bmp);;All Format(*.*)");
    QString promt = QString::fromLocal8Bit("打开图片");
    filePath = QFileDialog::getOpenFileName(this, promt, "", filter);

    if(filePath==NULL) return;

    QChar sep('/');
    QString title = filePath.split(sep).back();
    label->setText(title);
    imgWidget = new ImgWidget(this);
    if(!imgWidget->open(filePath.toStdString())){
        QString promt = QString::fromLocal8Bit("错误");
        QString msg = QString::fromLocal8Bit("无法打开图片 ");
        QMessageBox::critical(this,promt, msg+filePath,QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QMessageBox::information(this,tr("a"),QString::fromStdString(num2str(imgWidget->mat->channels())),QMessageBox::Yes);

    scrollArea->setWidget(imgWidget);
}

void MainWindow::on_action_Close_triggered()
{
    if(imgWidget == NULL) return;
    if(!imgWidget->isSaved){
        QChar sep('/');
        QString fileName = filePath.split(sep).back();
        QString promt = QString::fromLocal8Bit("警告");
        QString msg0 = QString::fromLocal8Bit("是否在关闭前保存对 ");
        QString msg1 = QString::fromLocal8Bit(" 的更改?");
        QMessageBox::StandardButton btn = QMessageBox::warning(this,promt,msg0+fileName+msg1,
                                          QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);
        if(btn ==  QMessageBox::Yes){
            imgWidget->save();
        }
        else if(btn == QMessageBox::Cancel){
            return;
        }
    }
    delete imgWidget;
    imgWidget = NULL;
    QString s[6]={"","","","","",""};
    updateMouseInfo(s);
    scrollArea->setWidget(NULL);
}

void MainWindow::on_action_Save_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(!imgWidget->save()){
        QString promt = QString::fromLocal8Bit("错误");
        QString msg = QString::fromLocal8Bit("无法保存图片");
        QString end = QString::fromLocal8Bit("，请确保路径中无中文");
        QMessageBox::critical(this,promt, msg+filePath+end,QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
}

void MainWindow::on_action_Save_As_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    QString filter = tr("JPEG(*.jpg;*.jpeg;*.jpe);;PNG(*.png;*.pns);;RAW(*.raw);;BITMAP(*.bmp);;All Format(*.*)");
    QString promt = QString::fromLocal8Bit("打开图片");
    filePath = QFileDialog::getSaveFileName(this, promt, "", filter);
    if(filePath==NULL) return;

    QChar sep('/');
    QString title = filePath.split(sep).back();
    label->setText(title);
    if(!imgWidget->saveAs(filePath.toStdString())){
        QString promt = QString::fromLocal8Bit("错误");
        QString msg = QString::fromLocal8Bit("无法保存图片");
        QString end = QString::fromLocal8Bit("，请确保路径中无中文");
        QMessageBox::critical(this,promt, msg+filePath+end,QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
}

void MainWindow::on_action_Exit_triggered()
{
    on_action_Close_triggered();
    exit(0);
}

void MainWindow::on_action_Undo_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    imgWidget->undo();
}

void MainWindow::on_action_Redo_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    imgWidget->redo();
}

void MainWindow::on_action_Help_triggered()
{

}

void MainWindow::on_action_About_triggered()
{

}


void MainWindow::on_rgb_gray_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!cvtRGB2GRAY(imgWidget->mat,dst))return;
    imgWidget->updateImg(dst);
}



void MainWindow::on_adjust_HSL_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=3)return;
    HSLDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_adjust_color_levels_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;

}

void MainWindow::on_channel_red_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!cvtRGB2SC(imgWidget->mat, dst, CHANNEL_R))return;
    imgWidget->updateImg(dst);
}

void MainWindow::on_channel_green_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!cvtRGB2SC(imgWidget->mat, dst, CHANNEL_G))return;
    imgWidget->updateImg(dst);
}

void MainWindow::on_channel_blue_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!cvtRGB2SC(imgWidget->mat, dst, CHANNEL_B))return;
    imgWidget->updateImg(dst);
}

void MainWindow::on_binary_Otsu_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!ostuBinary(imgWidget->mat, dst))return;
    imgWidget->updateImg(dst);
}

void MainWindow::on_binary_handson_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryDialog dialog(this);
    dialog.exec();
}
