#ifndef CUTDIALOG_H
#define CUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QBoxLayout>
#include <QDialogButtonBox>

class CutDialog: public QDialog
{
    Q_OBJECT

public:
    CutDialog(QWidget* parent = 0);
    ~CutDialog();

private slots:
    void accept();
    void reject();

private:
    QVBoxLayout* layout;
    QLabel *label_px, *label_py, *label_cw, *label_ch;
    QSpinBox *px, *py, *cw, *ch;
    QDialogButtonBox* button;

};

#endif // CUTDIALOG_H
