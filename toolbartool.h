#ifndef TOOLBARTOOL_H
#define TOOLBARTOOL_H

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QButtonGroup>
#include <QPushButton>

#define BTN_CURSOR      0
#define BTN_ALGEBRA     1
#define BTN_ROTATE      2
#define BTN_SCALE       3
#define BTN_CUT         4
#define BTN_CONTRAST    5
#define BTN_FILTER      6
#define BTN_NOISE       7

class ToolbarTool: public QToolBar
{
    Q_OBJECT

public:
    ToolbarTool(const QString& title, QWidget* parent = 0);
    ~ToolbarTool();

private slots:
    void on_btn_cur_clicked();
    void on_btn_alg_clicked();
    void on_btn_rot_clicked();
    void on_btn_cut_clicked();
    void on_btn_sca_clicked();
    void on_btn_con_clicked();
    void on_btn_noise_clicked();
    void on_filter_normal_triggered();
    void on_filter_custom_triggered();

private:
    /*widgets*/
    QButtonGroup* group;
    QAction *sca_bilinear, *sca_nearest, *filter_normal, *filter_custom;
    QPushButton *btn_cur, *btn_alg, *btn_rot, *btn_sca,
                *btn_cut, *btn_con, *btn_filter, *btn_noise;

};

#endif // TOOLBARTOOL_H
