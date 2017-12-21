#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QLabel>

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
    QTabWidget *tabWidget;
    QLabel *label_posx, *label_posy;
    QLabel *label_red, *label_blue, *label_green;

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void closeEvent(QCloseEvent*);
//    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_action_New_triggered();
    void on_action_Open_triggered();
    void on_action_Close_triggered();
    void on_action_Save_triggered();
    void on_tab_Close_triggered(int);
    void on_action_Exit_triggered();
    void on_action_Undo_triggered();
    void on_action_Redo_triggered();
    void on_action_Help_triggered();
    void on_action_About_triggered();
    void on_action_Save_As_triggered();

private:
    Ui::MainWindow *ui;

    /*booleans*/
    std::vector<bool> isSaved;
    std::vector<QString> fileOpened;
    int total = 0;

    /*functions*/
    void close(int);
    void save(int);
    void saveAs(int);
};

#endif // MAINWINDOW_H
