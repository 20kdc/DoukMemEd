#include <QTextStream>
#include "doukstatusview.h"
#include "ui_doukstatusview.h"

DoukStatusView::DoukStatusView(LPA::Process * proc) : DoukView(proc, "Player State..."),
    ui(new Ui::DoukStatusView)
{
    // set up equip checkbox text
    QString cbEquipText[CB_EQUIPS_COUNT];
    for (int i = 0; i < CB_EQUIPS_COUNT; i++)
        cbEquipText[i] = QString("Equip %1").arg(i + 1);
    // read from equips.txt
    QFile equips("equips.txt");
    if (equips.open(QFile::ReadOnly)) {
        QTextStream in(&equips);
        for (int i = 0; i < CB_EQUIPS_COUNT; i++) {
            if (in.atEnd())
                break;
            cbEquipText[i] = in.readLine();
        }
        equips.close();
    }
    ui->setupUi(this);
    // put equip chechboxes into array
    cbEquips[0] = ui->cbEquip01;
    cbEquips[1] = ui->cbEquip02;
    cbEquips[2] = ui->cbEquip03;
    cbEquips[3] = ui->cbEquip04;
    cbEquips[4] = ui->cbEquip05;
    cbEquips[5] = ui->cbEquip06;
    cbEquips[6] = ui->cbEquip07;
    cbEquips[7] = ui->cbEquip08;
    cbEquips[8] = ui->cbEquip09;
    cbEquips[9] = ui->cbEquip10;
    cbEquips[10] = ui->cbEquip11;
    cbEquips[11] = ui->cbEquip12;
    cbEquips[12] = ui->cbEquip13;
    cbEquips[13] = ui->cbEquip14;
    cbEquips[14] = ui->cbEquip15;
    cbEquips[15] = ui->cbEquip16;
    // connect equip checkboxes & set text
    for (int i = 0; i < CB_EQUIPS_COUNT; i++) {
        cbEquips[i]->setText(cbEquipText[i]);
        connect(cbEquips[i], &QCheckBox::clicked, [=](bool checked) {
            setEquip(static_cast<uint32_t>(i), checked);
        });
    }
    on_btnReadMem_clicked();
    connect(ui->btnReadMem, &QPushButton::clicked, this, &DoukStatusView::on_btnReadMem_clicked);
    connect(ui->sbMaxHP, SIGNAL(valueChanged(int)), this, SLOT(on_sbMaxHP_valueChanged(int)));
    connect(ui->sbCurHP, SIGNAL(valueChanged(int)), this, SLOT(on_sbCurHP_valueChanged(int)));
    connect(ui->cbLockHP, SIGNAL(clicked(bool)), this, SLOT(on_cbLockHP_clicked(bool)));
    connect(ui->cbInfAmmo, SIGNAL(clicked(bool)), this, SLOT(on_cbInfAmmo_clicked(bool)));
    connect(ui->cbInfBoost, SIGNAL(clicked(bool)), this, SLOT(on_cbInfBoost_clicked(bool)));
}

bool DoukStatusView::getEquip(uint32_t e)
{
    if (e > 15)
        return false;
    uint16_t word;
    proc->readMemory(Doukutsu::Equips, &word, sizeof(uint16_t));
    return (word & BIT(e)) != 0;
}

void DoukStatusView::setEquip(uint32_t e, bool v)
{
    if (e > 15)
        return;
    uint16_t word;
    proc->readMemory(Doukutsu::Equips, &word, sizeof(uint16_t));
    uint16_t mask = static_cast<uint16_t>(BIT(e));
    word |= mask;
    if (!v)
        word ^= mask;
    proc->writeMemory(Doukutsu::Equips, &word, sizeof(uint16_t));
}

bool DoukStatusView::getWeapon(uint32_t e, Doukutsu::Weapon* w)
{
    if (e > 7)
        return false;
    uint32_t off = Doukutsu::WeaponsStart + sizeof(Doukutsu::Weapon) * static_cast<uint32_t>(e);
    proc->readMemory(off, w, sizeof(Doukutsu::Weapon));
    return true;
}

bool DoukStatusView::setWeapon(uint32_t e, Doukutsu::Weapon* w)
{
    if (e > 7)
        return false;
    uint32_t off = Doukutsu::WeaponsStart + sizeof(Doukutsu::Weapon) * static_cast<uint32_t>(e);
    proc->writeMemory(off, w, sizeof(Doukutsu::Weapon));
    return true;
}

void DoukStatusView::timerRefresh()
{
    uint32_t dword = 9999;
    if (ui->cbLockHP->isChecked()) {
        uint16_t word = static_cast<uint16_t>(ui->sbCurHP->value());
        proc->writeMemory(Doukutsu::HealthDisplayed, &word, sizeof(uint16_t));
        proc->writeMemory(Doukutsu::HealthCurrent, &word, sizeof(uint16_t));
    }
    if (ui->cbInfBoost->isChecked())
        proc->writeMemory(Doukutsu::BoosterFuel, &dword, sizeof(uint32_t));
    if (ui->cbInfAmmo->isChecked()) {
        uint32_t slot;
        proc->readMemory(Doukutsu::CurWeaponSlot, &slot, sizeof(uint32_t));
        Doukutsu::Weapon wpn;
        getWeapon(slot, &wpn);
        wpn.ammo = wpn.ammoMax;
        setWeapon(slot, &wpn);
    }
}

void DoukStatusView::on_btnReadMem_clicked()
{
    uint16_t word;
    proc->readMemory(Doukutsu::HealthMaximum, &word, sizeof(uint16_t));
    ui->sbMaxHP->setValue(word);
    proc->readMemory(Doukutsu::HealthCurrent, &word, sizeof(uint16_t));
    ui->sbCurHP->setValue(word);
    for (int i = 0; i < CB_EQUIPS_COUNT; i++)
        cbEquips[i]->setChecked(getEquip(static_cast<uint32_t>(i)));
}

void DoukStatusView::on_sbMaxHP_valueChanged(int arg1)
{
    uint16_t val = static_cast<uint16_t>(arg1);
    proc->writeMemory(Doukutsu::HealthMaximum, &val, sizeof(uint16_t));
}

void DoukStatusView::on_sbCurHP_valueChanged(int arg1)
{
    uint16_t val = static_cast<uint16_t>(arg1);
    proc->writeMemory(Doukutsu::HealthCurrent, &val, sizeof(uint16_t));
    proc->writeMemory(Doukutsu::HealthDisplayed, &val, sizeof(uint16_t));
    if (ui->cbLockHP->isChecked()) {
        ui->sbMaxHP->setValue(arg1);
    }
}

void DoukStatusView::on_cbLockHP_clicked(bool checked)
{
    if (!checked) {
        ui->sbMaxHP->setEnabled(true);
        return;
    }
    ui->sbMaxHP->setEnabled(!checked);
    ui->sbMaxHP->setValue(ui->sbCurHP->value());
}

void DoukStatusView::on_cbInfBoost_clicked(bool checked)
{
    if (checked)
        timerRefresh();
}

void DoukStatusView::on_cbInfAmmo_clicked(bool checked)
{
    if (checked)
        timerRefresh();
}


DoukStatusView::~DoukStatusView()
{
    delete ui;
}
