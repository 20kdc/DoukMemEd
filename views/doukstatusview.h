#ifndef DOUKSTATUSVIEW_H
#define DOUKSTATUSVIEW_H

#include <QWidget>
#include <QCheckBox>
#include "doukview.h"
#include "doukutsu.h"

namespace Ui {
class DoukStatusView;
}

#define CB_EQUIPS_COUNT 16
class DoukStatusView : public DoukView
{
    Q_OBJECT

public:
    explicit DoukStatusView(LPA::Process * proc);
    virtual ~DoukStatusView() override;
    virtual void timerRefresh() override;
    bool getEquip(uint32_t e);
    void setEquip(uint32_t e, bool v);
    bool getWeapon(uint32_t e, Doukutsu::Weapon* w);
    bool setWeapon(uint32_t e, Doukutsu::Weapon* w);
private slots:
    void on_btnReadMem_clicked();
    void on_sbMaxHP_valueChanged(int arg1);
    void on_sbCurHP_valueChanged(int arg1);
    void on_cbLockHP_clicked(bool checked);
    void on_cbInfBoost_clicked(bool checked);
    void on_cbInfAmmo_clicked(bool checked);

private:
    Ui::DoukStatusView *ui;
    QCheckBox *cbEquips[CB_EQUIPS_COUNT];
};

#endif // DOUKSTATUSVIEW_H
