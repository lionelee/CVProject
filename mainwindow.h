#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QLabel>
#include <QScrollArea>
#include "toolbarinfo.h"
#include "toolbartool.h"
#include "toolbaropt.h"
#include "imgwidget.h"
#include "imgprocess.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*widgets*/
    Ui::MainWindow *ui;
    QScrollArea* scrollArea = NULL;
    ImgWidget* imgWidget = NULL;
    ToolbarOpt  *toolbarOpt;
    ToolbarInfo *toolbarInfo;

    void updateMouseInfo(QString(&info)[6]);
    void rotate();
    void adjustContrast();
    void smoothFilter();
    void customFilter();
    void cutDialog();
    void scaleDialog();
    void addNoise();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void closeEvent(QCloseEvent*);
//    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_action_New_triggered();
    void on_action_Open_triggered();
    void on_action_Close_triggered();
    void on_action_Save_triggered();
    void on_action_Save_As_triggered();
    void on_action_Exit_triggered();
    void on_action_Undo_triggered();
    void on_action_Redo_triggered();

    void on_channel_red_triggered();
    void on_channel_green_triggered();
    void on_channel_blue_triggered();
    void on_rgb_gray_triggered();
    void on_adjust_HSL_triggered();
    void on_adjust_color_levels_triggered();

    void on_binary_Otsu_triggered();
    void on_binary_handson_triggered();
    void on_binary_reverse_triggered();

    void on_action_Sobel_triggered();
    void on_action_Laplacian_triggered();
    void on_action_Canny_triggered();

    void on_binary_ersion_triggered();
    void on_binary_dilation_triggered();
    void on_binary_open_triggered();
    void on_binary_close_triggered();
    void on_hit_miss_triggered();
    void on_binary_thinning_triggered();
    void on_binary_thicking_triggered();
    void on_thining_serial_triggered();
    void on_thickening_serial_triggered();
    void on_binary_skeleton_triggered();
    void on_ske_reconstruct_triggered();
    void on_Euler_distance_triggered();
    void on_cblock_distance_triggered();
    void on_cboard_distance_triggered();
    void on_open_rebuild_triggered();
    void on_close_rebuild_triggered();

    void on_gray_ersion_triggered();
    void on_gray_dilation_triggered();
    void on_gray_open_triggered();
    void on_gray_close_triggered();
    void on_gray_watershed_triggered();
    void on_grayOpen_rebuild_triggered();
    void on_grayClose_rebuild_triggered();

    void on_Hough_Line_triggered();

private:
    ToolbarTool *toolbarTool;
    QLabel *label;
    QString filePath;

};

#endif // MAINWINDOW_H
