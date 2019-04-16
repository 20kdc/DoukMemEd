#include "doukmemed.h"
#include "ui_doukmemed.h"
#include <QWindow>
#include <QMessageBox>
#include <iostream>
#include <string>
#include <sstream>
#include <stdint.h>
#include <QTextStream>
#include "views/doukview.h"
#include "views/doukmapview.h"
#include "views/doukstatusview.h"
#include "doukutsu.h"
using namespace std;

DoukMemEd::DoukMemEd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DoukMemEd)
{
    // call setupUi
    ui->setupUi(this);
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

        // Note that proc starts out nullptr, and is only set if a t-candidate is OK.
        // Failed t-candidates are immediately deleted.
        for (intptr_t ip : processes) {
            LPA::Process * t = new LPA::Process(this, ip);
            if ((t->isValid()) && (t->matchesNameTemplate(exeName))) {
                cout << "Attempting attach to PID " << t->getPID() << endl;
                if (!t->canBeginMemoryAccess()) {
                    delete t;
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
                proc = t;
                break;
            }
            delete t;
        }

        if (!proc) {
            QMessageBox::critical(this, QString("Attach fail"), QString(
                "Could not find Cave Story process.\n"
                "Make sure Cave Story is running, and that its name is %1.\n"
#ifdef Q_OS_WIN
                "If it is running, it might be elevated. Run Doukutsu Memory Editor as an Administrator and try again."
#endif
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
        lockUpdateTimer->start(100); // locks are updated every 1/10th of a second
        ui->leExeName->setDisabled(true);
        ui->btnAttach->setText(QString("Detach"));
        // -- Install All Views
        views.append(new DoukMapView(proc));
        views.append(new DoukStatusView(proc));
        for (DoukView * dv : views) {
            QPushButton * pb = new QPushButton(dv->buttonName);
            connect(pb, &QPushButton::clicked, dv, &DoukView::show);
            ui->vViews->layout()->addWidget(pb);
            viewButtons.append(pb);
        }
        // --
        setWindowTitle(QString("Doukutsu Memory Editor [Attached]"));
    } else
        detach();
}

void DoukMemEd::detach() {
    // About to delete proc, inform dialogs that they need to close.
    // -- Uninstall All Views
    for (QPushButton * pb : viewButtons) {
        ui->vViews->layout()->removeWidget(pb);
        pb->deleteLater();
    }
    viewButtons.clear();
    for (DoukView * dv : views)
        delete dv;
    views.clear();
    // --
    if (proc)
        delete proc;
    proc = nullptr;
    cout << "Detached from Cave Story process.\r\n";
    lockUpdateTimer->stop();
    ui->leExeName->setDisabled(false);
    ui->btnAttach->setText(QString("Attach"));
    setWindowTitle(QString("Doukutsu Memory Editor"));
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
    for (DoukView * dv : views)
        dv->timerRefresh();
}
