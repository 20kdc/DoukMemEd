#include "doukmemed.h"
#include "ui_doukmemed.h"
#include <QMessageBox>
#include <iostream>
#include <string>
#include <sstream>
#include <stdint.h>
#include <QTextStream>
#include "doukutsu.h"
using namespace std;

DoukMemEd::DoukMemEd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DoukMemEd)
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
    // call setupUi
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
            setEquip(static_cast<uint32_t>(i), &checked);
        });
    }
    // create lock update timer & connect
    lockUpdateTimer = new QTimer(this);
    connect(lockUpdateTimer, &QTimer::timeout, this, &DoukMemEd::updateLocks);
}

DoukMemEd::~DoukMemEd()
{
    detach();
    delete ui;
}

void DoukMemEd::on_btnAttach_clicked()
{
    if (!proc) {
        // correct EXE name string, if needed
        QString exeName = ui->leExeName->text();
        QString exeExt = ".exe";
        if (!exeName.endsWith(exeExt)) {
            int dotIndex = exeName.lastIndexOf('.');
            if (dotIndex == -1)
                exeName += exeExt;
            else {
                QString sub = exeName.mid(dotIndex, exeName.length() - dotIndex);
                if (exeExt.startsWith(sub)) {
                    int sLen = sub.length();
                    exeName += exeExt.mid(sLen, exeName.length() - sLen);
                } else
                    exeName += exeExt;
            }
            ui->leExeName->setText(exeName);
        }
        // attach
        QList<intptr_t> processes = QList<intptr_t>();
        if(!LPA::getProcesses(processes)) {
            QMessageBox::critical(this, QString("Attach fail"), QString("Process enumeration failed, error code %1.").arg(LPA::getLastError()));
            return;
        }

        proc = nullptr;
        for (intptr_t ip : processes) {
            proc = new LPA::Process(ip);
            if ((proc->isValid()) && (proc->matchesNameTemplate(exeName))) {
                cout << "Attempting attach to PID " << proc->getPID() << endl;
                if (!proc->canBeginMemoryAccess()) {
                    delete proc;
                    QMessageBox::critical(this, QString("Attach fail"), QString(
                        "The Cave Story process was found, but access to memory was not available.\n"
#ifdef Q_OS_WIN
                        "It is possible that the wrong target was chosen, or the target was run as Administrator or another user and this program wasn't.\n"
#else
                        "It is possible that the wrong target was chosen, or the target was run as another user.\n"
#endif
#ifdef Q_OS_LINUX
                        // Canonical, may I ask how YAMA is added security? If something can go ptracing arbitrary targets, it can just as well read the data off disk.
                        "On certain Linux kernel builds, /proc/sys/kernel/yama/ptrace_scope must be set to 0 to allow ptrace between user processes.\n"
                        "This will apply until the next reboot.\n"
                        "This must be done with \"su\" followed by \"echo 0 > /proc/sys/kernel/yama/ptrace_scope\".\n"
#endif
                        "The specific situation depends on various factors, though."
                        ));
                    return;
                }
                break;
            }
            delete proc;
            proc = nullptr;
        }

        if (!proc) {
            QMessageBox::critical(this, QString("Attach fail"), QString(
                "Could not find Cave Story process.\n"
                "Make sure Cave Story is running, and that its name is %1.\n"
                "If it is running, it might be elevated. Run Doukutsu Memory Editor as an Administrator and try again."
            ).arg(ui->leExeName->text()));
            return;
        }

        cout << "Attached to process, PID: " << proc->getPID() << "." << endl;
        uint8_t ver[5] = {0xA1, 0x20, 0x8B, 0x49, 0x00};
        uint8_t dat[5];
        proc->readMemory(Doukutsu::VerifyExe, dat, 5);
        if (memcmp(dat, ver, 5)) {
            QMessageBox::StandardButton btn = QMessageBox::warning(this, QString("Non-Cave Story process"), QString("Process does not seem to be of a Cave Story executable.\nAttach anyway?"),  QMessageBox::Yes | QMessageBox::No);
            if (btn != QMessageBox::Yes) {
                delete proc;
                proc = nullptr;
                return;
            }
        }
        cout << "Successfully attached to Cave Story process (PID: " << proc->getPID() << ")." << endl;
        disableLocks();
        lockUpdateTimer->start(100); // locks are updated every 1/10th of a second
        ui->leExeName->setDisabled(true);
        ui->btnAttach->setText(QString("Detach"));
        ui->btnReadMem->setDisabled(false);
        setWindowTitle(QString("Doukutsu Memory Editor [Attached]"));
        setWidgetsDisabled(false);
        on_btnReadMem_clicked();
    } else
        detach();
}

void DoukMemEd::detach() {
    if (proc)
        delete proc;
    proc = nullptr;
    cout << "Detached from Cave Story process.\r\n";
    lockUpdateTimer->stop();
    ui->leExeName->setDisabled(false);
    ui->btnAttach->setText(QString("Attach"));
    ui->btnReadMem->setDisabled(true);
    setWindowTitle(QString("Doukutsu Memory Editor"));
    setWidgetsDisabled(true);
}

void DoukMemEd::disableLocks() {
    ui->cbLockHP->setChecked(false);
    ui->cbInfBoost->setChecked(false);
    ui->cbInfAmmo->setChecked(false);
}

void DoukMemEd::setWidgetsDisabled(bool v) {
    ui->sbMaxHP->setDisabled(v);
    ui->sbCurHP->setDisabled(v);
    ui->cbLockHP->setDisabled(v);
    for (int i = 0; i < CB_EQUIPS_COUNT; i++)
        cbEquips[i]->setDisabled(v);
    ui->cbInfBoost->setDisabled(v);
    ui->cbInfAmmo->setDisabled(v);
}

bool DoukMemEd::checkProcStillRunning() {
    if (!proc)
        return false;
    if (proc->isStillAlive())
        return true;
    QMessageBox::information(this, QString("Doukutsu process dead"), QString("The Cave Story process has ended."));
    detach();
    return false;
}

void DoukMemEd::updateLocks() {
    if (!checkProcStillRunning())
        return;
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

bool DoukMemEd::getEquip(uint32_t e, bool* v) {
    if (e > 15)
        return false;
    uint16_t word;
    proc->readMemory(Doukutsu::Equips, &word, sizeof(uint16_t));
    *v = (word & BIT(e)) != 0;
    return true;
}

bool DoukMemEd::setEquip(uint32_t e, bool* v) {
    if (e > 15)
        return false;
    uint16_t word;
    proc->readMemory(Doukutsu::Equips, &word, sizeof(uint16_t));
    uint16_t mask = static_cast<uint16_t>(BIT(e));
    if (v)
        word |= mask;
    else
        word ^= mask;
    proc->writeMemory(Doukutsu::Equips, &word, sizeof(uint16_t));
    return true;
}

bool DoukMemEd::getWeapon(uint32_t e, Doukutsu::Weapon* w) {
    if (e > 7)
        return false;
    uint32_t off = Doukutsu::WeaponsStart + sizeof(Doukutsu::Weapon) * static_cast<uint32_t>(e);
    proc->readMemory(off, w, sizeof(Doukutsu::Weapon));
    return true;
}

bool DoukMemEd::setWeapon(uint32_t e, Doukutsu::Weapon* w) {
    if (e > 7)
        return false;
    uint32_t off = Doukutsu::WeaponsStart + sizeof(Doukutsu::Weapon) * static_cast<uint32_t>(e);
    proc->writeMemory(off, w, sizeof(Doukutsu::Weapon));
    return true;
}

void DoukMemEd::movePlayer(int xm, int ym) {
    int x;
    if (xm != 0) {
        proc->readMemory(Doukutsu::PlayerX, &x, sizeof(int));
        x += xm * 256;
        proc->writeMemory(Doukutsu::PlayerX, &x, sizeof(int));
    }
    int y;
    if (ym != 0) {
        proc->readMemory(Doukutsu::PlayerY, &y, sizeof(int));
        y += ym * 256;
        proc->writeMemory(Doukutsu::PlayerY, &y, sizeof(int));
    }
}

void DoukMemEd::on_btnReadMem_clicked()
{
    if (!checkProcStillRunning())
        return;
    uint16_t word;
    proc->readMemory(Doukutsu::HealthMaximum, &word, sizeof(uint16_t));
    ui->sbMaxHP->setValue(word);
    proc->readMemory(Doukutsu::HealthCurrent, &word, sizeof(uint16_t));
    ui->sbCurHP->setValue(word);
    bool b = false;
    for (int i = 0; i < CB_EQUIPS_COUNT; i++) {
        getEquip(static_cast<uint32_t>(i), &b);
        cbEquips[i]->setChecked(b);
    }
}

void DoukMemEd::on_sbMaxHP_valueChanged(int arg1)
{
    if (!checkProcStillRunning())
        return;
    uint16_t val = static_cast<uint16_t>(arg1);
    proc->writeMemory(Doukutsu::HealthMaximum, &val, sizeof(uint16_t));
}

void DoukMemEd::on_sbCurHP_valueChanged(int arg1)
{
    if (!checkProcStillRunning())
        return;
    uint16_t val = static_cast<uint16_t>(arg1);
    proc->writeMemory(Doukutsu::HealthCurrent, &val, sizeof(uint16_t));
    proc->writeMemory(Doukutsu::HealthDisplayed, &val, sizeof(uint16_t));
    if (ui->cbLockHP->isChecked()) {
        ui->sbMaxHP->setValue(arg1);
    }
}

void DoukMemEd::on_cbLockHP_clicked(bool checked)
{
    if (!checked) {
        ui->sbMaxHP->setEnabled(true);
        return;
    }
    ui->sbMaxHP->setEnabled(!checked);
    ui->sbMaxHP->setValue(ui->sbCurHP->value());
}

void DoukMemEd::on_cbInfBoost_clicked(bool checked)
{
    if (checked)
        updateLocks();
}

void DoukMemEd::on_cbInfAmmo_clicked(bool checked)
{
    if (checked)
        updateLocks();
}
