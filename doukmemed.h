#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QCheckBox>
#include "lpa.h"

#define CB_EQUIPS_COUNT 16

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
    void on_cbLockHP_clicked(bool checked);
    void updateLocks();

private:
    Ui::DoukMemEd *ui;
    LPA::Process *proc = nullptr;
    void detach();
    bool checkProcStillRunning();
    void setWidgetsDisabled(bool v);
    bool getEquip(int e);
    void setEquip(int e, bool v);
    QTimer *lockUpdateTimer;
    QCheckBox *cbEquips[CB_EQUIPS_COUNT];
};
