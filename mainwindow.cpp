#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hsldialog.h"
#include "colorleveldialog.h"
#include "rotatedialog.h"
#include "cutdialog.h"
#include "scaledialog.h"
#include "binarydialog.h"
#include "contrastdialog.h"
#include "filterdialog.h"
#include "customfilter.h"
#include "sobeldialog.h"
#include "cannydialog.h"
#include "noisedialog.h"
#include "binarymorphology.h"
#include "graymorphology.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("CVImgProcessor"));
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

void MainWindow::adjustContrast()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    ContrastDialog dialog(this);
    dialog.exec();
}

void MainWindow::rotate()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    RotateDialog dialog(this);
    dialog.exec();
    std::string str = num2str(imgWidget->mat->rows)+"x"+num2str(imgWidget->mat->cols);
    toolbarInfo->updateSize(str);
    toolbarInfo->updateChannel(num2str(imgWidget->mat->channels()));
}

void MainWindow::smoothFilter()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    FilterDialog dialog(this);
    dialog.exec();
}

void MainWindow::customFilter()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    CustomFilter dialog(this);
    dialog.exec();
}


void MainWindow::cutDialog()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    CutDialog dialog(this);
    dialog.exec();
}

void MainWindow::scaleDialog()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    ScaleDialog dialog(this);
    dialog.exec();
}

void MainWindow::addNoise()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    NoiseDialog dialog(this);
    dialog.exec();
}

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
    MainWindow* mw = new MainWindow(parentWidget());
    mw->show();
}


void MainWindow::on_action_Open_triggered()
{
    if(imgWidget != NULL)return;
    QString filter = tr("JPEG(*.jpg;*.jpeg;*.jpe);;PNG(*.png;*.pns);;TIFF(*.tiff);;BITMAP(*.bmp);;All Format(*.*)");
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
        if(imgWidget!=NULL){
            imgWidget->resize(100, 100);
            delete imgWidget;
            imgWidget = NULL;
        }
        return;
    }

    std::string str_size = num2str(imgWidget->mat->rows)+"x"+num2str(imgWidget->mat->cols);
    std::string channel = num2str(imgWidget->mat->channels());
    toolbarInfo->updateImageInfo(filePath, str_size, channel);
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
    imgWidget->resize(100, 100);
    delete imgWidget;
    imgWidget = NULL;
    QString s[6]={"","","","","",""};
    toolbarInfo->updateMouseInfo(s);
    filePath = tr("");
    toolbarInfo->updateImageInfo(filePath,"","");
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
    QString filter = tr("JPEG(*.jpg;*.jpeg;*.jpe);;PNG(*.png;*.pns);;TIFF(*.tiff);;BITMAP(*.bmp);;All Format(*.*)");
    QString promt = QString::fromLocal8Bit("打开图片");
    QString savePath = QFileDialog::getSaveFileName(this, promt, "", filter);
    if(savePath==NULL) return;
    QChar sep('/');
    QString title = savePath.split(sep).back();
    label->setText(title);
    if(!imgWidget->saveAs(savePath.toStdString())){
        QString promt = QString::fromLocal8Bit("错误");
        QString msg = QString::fromLocal8Bit("无法保存图片");
        QString end = QString::fromLocal8Bit("，请确保路径中无中文");
        QMessageBox::critical(this,promt, msg+savePath+end,QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    filePath = savePath;
    std::string str_size = num2str(imgWidget->mat->rows)+"x"+num2str(imgWidget->mat->cols);
    std::string channel = num2str(imgWidget->mat->channels());
    toolbarInfo->updateImageInfo(filePath, str_size, channel);
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
    std::string str = num2str(imgWidget->mat->rows)+"x"+num2str(imgWidget->mat->cols);
    toolbarInfo->updateSize(str);
    toolbarInfo->updateChannel(num2str(imgWidget->mat->channels()));
}

void MainWindow::on_action_Redo_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    imgWidget->redo();
    std::string str = num2str(imgWidget->mat->rows)+"x"+num2str(imgWidget->mat->cols);
    toolbarInfo->updateSize(str);
    toolbarInfo->updateChannel(num2str(imgWidget->mat->channels()));
}

void MainWindow::on_rgb_gray_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!cvtRGB2GRAY(imgWidget->mat,dst))return;
    imgWidget->updateImg(dst);
    toolbarInfo->updateChannel("1");
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
    if(imgWidget->mat->channels()!=3)return;
    ColorLevelDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_channel_red_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!cvtRGB2SC(imgWidget->mat, dst, CHANNEL_R))return;
    imgWidget->updateImg(dst);
    toolbarInfo->updateChannel("1");
}

void MainWindow::on_channel_green_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!cvtRGB2SC(imgWidget->mat, dst, CHANNEL_G))return;
    imgWidget->updateImg(dst);
    toolbarInfo->updateChannel("1");
}

void MainWindow::on_channel_blue_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!cvtRGB2SC(imgWidget->mat, dst, CHANNEL_B))return;
    imgWidget->updateImg(dst);
    toolbarInfo->updateChannel("1");
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

void MainWindow::on_binary_reverse_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* dst = new Mat(imgWidget->mat->rows, imgWidget->mat->cols, CV_8UC1);
    if(!Reverse(imgWidget->mat, dst))return;
    imgWidget->updateImg(dst);
}


void MainWindow::on_action_Sobel_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    SobelDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_action_Laplacian_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    LaplacianDetect(src, dst);
    imgWidget->updateImg(dst);
}

void MainWindow::on_action_Canny_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    CannyDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_binary_ersion_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, EROSION);
    dialog.exec();
}

void MainWindow::on_binary_dilation_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, DILATION);
    dialog.exec();
}

void MainWindow::on_binary_open_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, OPEN);
    dialog.exec();
}

void MainWindow::on_binary_close_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, CLOSE);
    dialog.exec();
}

void MainWindow::on_hit_miss_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, HITMISS);
    dialog.exec();
}


void MainWindow::on_binary_thinning_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, THINNING);
    dialog.exec();
}

void MainWindow::on_binary_thicking_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, THICKING);
    dialog.exec();
}

void MainWindow::on_thining_serial_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    Mat* src = imgWidget->mat;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    thinning(src, dst);
    imgWidget->updateImg(dst);
}

void MainWindow::on_thickening_serial_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    Mat* src = imgWidget->mat;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    thickening(src, dst);
    imgWidget->updateImg(dst);
}

void MainWindow::on_binary_skeleton_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, SKELETON);
    dialog.exec();
}

void MainWindow::on_ske_reconstruct_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, SKERECON);
    dialog.exec();
}

void MainWindow::on_Euler_distance_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    if(!distanceTrans(src, dst, EULERDIS))return;
    imgWidget->updateImg(dst);
}

void MainWindow::on_cblock_distance_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    if(!distanceTrans(src, dst, CBLOCKDIS))return;
    imgWidget->updateImg(dst);
}

void MainWindow::on_cboard_distance_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    if(!distanceTrans(src, dst, CBOARDDIS))return;
    imgWidget->updateImg(dst);
}

void MainWindow::on_erosion_rebuild_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, EROSRBD);
    dialog.exec();
}

void MainWindow::on_dilation_rebuild_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, DILARBD);
    dialog.exec();
}

void MainWindow::on_open_rebuild_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, OPENRBD);
    dialog.exec();
}

void MainWindow::on_close_rebuild_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    if(imgWidget->mat->channels()!=1)return;
    BinaryMorphology dialog(this, CLOSERBD);
    dialog.exec();
}

void MainWindow::on_gray_ersion_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    GrayMorphology dialog(this, GEROSION);
    dialog.exec();
}

void MainWindow::on_gray_dilation_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    GrayMorphology dialog(this, GDILATION);
    dialog.exec();
}

void MainWindow::on_gray_open_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    GrayMorphology dialog(this, GOPEN);
    dialog.exec();
}

void MainWindow::on_gray_close_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    GrayMorphology dialog(this, GCLOSE);
    dialog.exec();
}

void MainWindow::on_grayErosion_rebuild_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    GrayMorphology dialog(this, GEROSRBD);
    dialog.exec();
}

void MainWindow::on_grayDilation_rebuild_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    GrayMorphology dialog(this, GDILARBD);
    dialog.exec();
}

void MainWindow::on_grayOpen_rebuild_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    GrayMorphology dialog(this, GOPENRBD);
    dialog.exec();

}

void MainWindow::on_grayClose_rebuild_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    GrayMorphology dialog(this, GCLOSERBD);
    dialog.exec();
}

void MainWindow::on_Hough_Line_triggered()
{
    if(imgWidget==NULL || imgWidget->mat==NULL)return;
    Mat* src = imgWidget->mat;
    if(src->channels()!=1)return;
    Mat* dst = new Mat(src->rows, src->cols, CV_8UC1);
    bool ok = false;
    int n = QInputDialog::getInt(this,tr("InputDialog"),tr("threshold:"),10,1,1000,1,&ok);
    if(ok){
        HoughLine(src, dst, n);
        imgWidget->updateImg(dst);
    }
}
