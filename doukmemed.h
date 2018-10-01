#ifndef DOUKMEMED_H
#define DOUKMEMED_H

#include <QMainWindow>
#include "windows.h"
#include <QTimer>

namespace Ui {
class DoukMemEd;
}

class DoukMemEd : public QMainWindow
{
    Q_OBJECT

public:
    explicit DoukMemEd(QWidget *parent = nullptr);
    ~DoukMemEd();

private slots:
    void on_btnAttach_clicked();
    void on_btnReadMem_clicked();
    void on_sbMaxHP_valueChanged(int arg1);
    void on_sbCurHP_valueChanged(int arg1);
    void updateLocks();

    void on_cbLockHP_clicked(bool checked);

private:
    Ui::DoukMemEd *ui;
    HANDLE proc = nullptr;
    void detach();
    bool checkProcStillRunning();
    void setWidgetsDisabled(bool v);
    QTimer *lockUpdateTimer;
};

#endif // DOUKMEMED_H
