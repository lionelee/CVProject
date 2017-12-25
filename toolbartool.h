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
    void on_sca_bilinear_triggered();
    void on_sca_nearest_triggered();

private:
    /*widgets*/
    QButtonGroup* group;
    QAction *sca_bilinear, *sca_nearest;
    QPushButton *btn_cur, *btn_alg, *btn_rot, *btn_sca, *btn_cut;

};

#endif // TOOLBARTOOL_H
