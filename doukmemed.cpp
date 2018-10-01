#include "doukmemed.h"
#include "ui_doukmemed.h"
#include <QMessageBox>
#include <iostream>
#include <psapi.h>
#include <string>
#include <sstream>
#include "doukutsu.h"
using namespace std;

DoukMemEd::DoukMemEd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DoukMemEd)
{
    ui->setupUi(this);
    lockUpdateTimer = new QTimer(this);
    connect(lockUpdateTimer, SIGNAL(timeout()), this, SLOT(updateLocks()));
}

DoukMemEd::~DoukMemEd()
{
    delete ui;
}

void DoukMemEd::on_btnAttach_clicked()
{
    if (proc == nullptr) {
        // correct EXE name string, if needed
        QString exeName = ui->leExeName->text();
        if (!exeName.endsWith(".exe")) {
            exeName += ".exe";
            ui->leExeName->setText(exeName);
        }
        // attach
        DWORD *pids;
        if ((pids = new DWORD[1024]) == nullptr) {
            QMessageBox::critical(this, QString("Attach fail"), QString("Could not allocate enough memory to enumerate processes.\nI only wanted 4096 bytes..."));
            return;
        }

        DWORD szRet;
        if(!EnumProcesses(pids, sizeof(DWORD)*1024, &szRet)) {
            QMessageBox::critical(this, QString("Attach fail"), QString("Process enumeration failed, error code %1.").arg(GetLastError()));
            return;
        }

        char name[513];
        string doukutsu = exeName.toUtf8().constData();
        DWORD i = 0;
        for (; i < szRet; i++) {
            proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, true, pids[i]);
            if (proc != nullptr) {
                DWORD ns = GetModuleFileNameExA(proc, nullptr, name, 512);
                if (ns != 0) {
                    char* nameStart = strrchr(name, '\\') + 1;
                    if (nameStart == doukutsu)
                        break;
                }
                CloseHandle(proc);
                proc = nullptr;
            }
        }

        if (proc == nullptr) {
            QMessageBox::critical(this, QString("Attach fail"), QString("Could not find Cave Story process.\nMake sure Cave Story is running, and that its name is %1.").arg(ui->leExeName->text()));
            return;
        }

        cout << "Successfully attached to Cave Story process (PID: " << pids[i] << ").\r\n";
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
    if (proc != nullptr)
        CloseHandle(proc);
    proc = nullptr;
    cout << "Detached from Cave Story process.\r\n";
    lockUpdateTimer->stop();
    ui->leExeName->setDisabled(false);
    ui->btnAttach->setText(QString("Attach"));
    ui->btnReadMem->setDisabled(true);
    setWindowTitle(QString("Doukutsu Memory Editor"));
    setWidgetsDisabled(true);
}

void DoukMemEd::setWidgetsDisabled(bool v) {
    if (v)
        ui->cbLockHP->setChecked(false);
    ui->sbMaxHP->setDisabled(v);
    ui->sbCurHP->setDisabled(v);
    ui->cbLockHP->setDisabled(v);
}

bool DoukMemEd::checkProcStillRunning() {
    if (proc == nullptr)
        return false;
    DWORD code;
    GetExitCodeProcess(proc, &code);
    if (code == STILL_ACTIVE)
        return true;
    QMessageBox::information(this, QString("Doukutsu process dead"), QString("The Cave Story process has ended."));
    detach();
    return false;
}

void DoukMemEd::updateLocks() {
    if (!checkProcStillRunning())
        return;
    if (ui->cbLockHP->isChecked()) {
        WORD word = static_cast<WORD>(ui->sbCurHP->value());
        // max health also has to be set to this value
        // (max HP spinbox is disabled by lock checkbox being toggled on)
        ui->sbMaxHP->setValue(word);
        WriteProcessMemory(proc, ptr(CS_health_maximum), &word, sizeof(WORD), nullptr);
        WriteProcessMemory(proc, ptr(CS_health_displayed), &word, sizeof(WORD), nullptr);
        WriteProcessMemory(proc, ptr(CS_health_current), &word, sizeof(WORD), nullptr);
    }
}

void DoukMemEd::on_btnReadMem_clicked()
{
    if (!checkProcStillRunning())
        return;
    WORD word;
    ReadProcessMemory(proc, ptr(CS_health_maximum), &word, sizeof(WORD), nullptr);
    ui->sbMaxHP->setValue(word);
    ReadProcessMemory(proc, ptr(CS_health_current), &word, sizeof(WORD), nullptr);
    ui->sbCurHP->setValue(word);
}

void DoukMemEd::on_sbMaxHP_valueChanged(int arg1)
{
    if (!checkProcStillRunning())
        return;
    WriteProcessMemory(proc, ptr(CS_health_maximum), &arg1, sizeof(WORD), nullptr);
}

void DoukMemEd::on_sbCurHP_valueChanged(int arg1)
{
    if (!checkProcStillRunning())
        return;
    WriteProcessMemory(proc, ptr(CS_health_current), &arg1, sizeof(WORD), nullptr);
    WriteProcessMemory(proc, ptr(CS_health_displayed), &arg1, sizeof(WORD), nullptr);
    if (ui->cbLockHP->isChecked())
        WriteProcessMemory(proc, ptr(CS_health_maximum), &arg1, sizeof(WORD), nullptr);
}

void DoukMemEd::on_cbLockHP_clicked(bool checked)
{
    ui->sbMaxHP->setEnabled(!checked);
}
