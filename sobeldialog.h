#ifndef SOBELDIALOG_H
#define SOBELDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QToolButton>
#include <QDialogButtonBox>

#define BTN_HORIZONTAL  0
#define BTN_VERTICAL    1
#define BTN_NORTHWEST   2
#define BTN_NORTHEAST   3

class SobelDialog: public QDialog
{
    Q_OBJECT

public:
    SobelDialog(QWidget* parent = 0);
    ~SobelDialog();

private slots:
    void accept();
    void reject();
    void on_btn_toggled(int id, bool stat);

private:
    QVBoxLayout* layout;
    QHBoxLayout* hlayout;
    QDialogButtonBox* button;
    QButtonGroup* group;
    QToolButton *btn_ho, *btn_ve, *btn_nw, *btn_ne;

};

#endif // SOBELDIALOG_H
