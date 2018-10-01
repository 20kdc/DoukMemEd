#include "doukmemed.h"
#include "ui_doukmemed.h"
#include <QMessageBox>
#include <iostream>
#include <string>
#include <sstream>
#include <stdint.h>
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
#ifdef WINDOWS
                        "It is possible that the wrong target was chosen, or the target was run as Administrator or another user and this program wasn't.\n"
#else
                        "It is possible that the wrong target was chosen, or the target was run as another user.\n"
#endif
#ifdef __linux__
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

        if (proc == nullptr) {
            QMessageBox::critical(this, QString("Attach fail"), QString("Could not find Cave Story process.\nMake sure Cave Story is running, and that its name is %1.").arg(ui->leExeName->text()));
            return;
        }

        cout << "Successfully attached to Cave Story process (PID: " << proc->getPID() << ")." << endl;
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
    if (proc->isStillAlive())
        return true;
    QMessageBox::information(this, QString("Doukutsu process dead"), QString("The Cave Story process has ended."));
    detach();
    return false;
}

void DoukMemEd::updateLocks() {
    if (!checkProcStillRunning())
        return;
    if (ui->cbLockHP->isChecked()) {
        uint16_t word = static_cast<uint16_t>(ui->sbCurHP->value());
        // max health also has to be set to this value
        // (max HP spinbox is disabled by lock checkbox being toggled on)
        ui->sbMaxHP->setValue(word);
        proc->writeMemory(CS_health_maximum, &word, sizeof(uint16_t));
        proc->writeMemory(CS_health_displayed, &word, sizeof(uint16_t));
        proc->writeMemory(CS_health_current, &word, sizeof(uint16_t));
    }
}

void DoukMemEd::on_btnReadMem_clicked()
{
    if (!checkProcStillRunning())
        return;
    uint16_t word;
    proc->readMemory(CS_health_maximum, &word, sizeof(uint16_t));
    ui->sbMaxHP->setValue(word);
    proc->readMemory(CS_health_current, &word, sizeof(uint16_t));
    ui->sbCurHP->setValue(word);
}

void DoukMemEd::on_sbMaxHP_valueChanged(int arg1)
{
    if (!checkProcStillRunning())
        return;
    uint16_t val = static_cast<uint16_t>(arg1);
    proc->writeMemory(CS_health_maximum, &val, sizeof(uint16_t));
}

void DoukMemEd::on_sbCurHP_valueChanged(int arg1)
{
    if (!checkProcStillRunning())
        return;
    uint16_t val = static_cast<uint16_t>(arg1);
    proc->writeMemory(CS_health_current, &val, sizeof(uint16_t));
    proc->writeMemory(CS_health_displayed, &val, sizeof(uint16_t));
    if (ui->cbLockHP->isChecked())
        proc->writeMemory(CS_health_maximum, &val, sizeof(uint16_t));
}

void DoukMemEd::on_cbLockHP_clicked(bool checked)
{
    ui->sbMaxHP->setEnabled(!checked);
}
