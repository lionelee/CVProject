#ifndef TOOLBAROPT_H
#define TOOLBAROPT_H

#include <QToolBar>
#include <QAction>
#include <vector>
#include "imgprocess.h"

#define CUR_OPT 0
#define ALG_OPT 1
#define ROT_OPT 2
#define CUT_OPT 3

class ToolbarOpt: public QToolBar
{
    Q_OBJECT

public:
    ToolbarOpt(const QString& title, QWidget* parent = 0);
    ~ToolbarOpt();

    void setCursorOpt();
    void setAlgebraOpt();
    void setCutOpt();

private slots:
    void on_act_add_triggered();
    void on_act_diff_triggered();
    void on_act_mul_triggered();

private:
    int idx = 0;
    std::vector<QAction*> cur_opt,alg_opt;
    QAction *act_add, *act_diff, *act_mul;

    void removeLast();
    bool openFile(Mat* dst);
    Mat* openFile();
};

#endif // TOOLBAROPT_H
