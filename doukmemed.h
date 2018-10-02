#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QCheckBox>
#include "doukutsu.h"
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

    void on_cbInfBoost_clicked(bool checked);

    void on_sbWepID_valueChanged(int arg1);

    void on_cbInfAmmo_clicked(bool checked);

private:
    Ui::DoukMemEd *ui;
    LPA::Process *proc = nullptr;
    void detach();
    bool checkProcStillRunning();
    void disableLocks();
    void setWidgetsDisabled(bool v);
    bool getEquip(uint32_t e);
    void setEquip(uint32_t e, bool v);
    void getWeapon(uint32_t slot, Doukutsu::Weapon* wpn);
    void setWeapon(uint32_t slot, Doukutsu::Weapon* wpn);
    QTimer *lockUpdateTimer;
    QCheckBox *cbEquips[CB_EQUIPS_COUNT];
};
