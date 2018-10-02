// This one's going to be super-easy.
// That said, for reference, see "man 5 proc", and "man 2 ptrace".

#include <QDir>
#include <string>
#include "lpa.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <QTextStream>

// handle is used for access to the process directory (done separately to somewhat minimize race condition stuff).
// handle2 is basically a bool used to indicate if we've taken over ptrace or not.

LPA::Process::Process(intptr_t pidx) {
    pid = pidx;
    valid = true;

    QString qs = QString("/proc/%1").arg(pidx);
    QByteArray qba = qs.toUtf8();

    handle = open(qba.constData(), O_RDONLY);
    if (handle < 0)
        valid = false;
    handle2 = -1;
}

LPA::Process::~Process() {
    if (valid) {
        close(static_cast<int>(handle));
        if (handle2 >= 0)
            close(static_cast<int>(handle2));
    }
}

bool LPA::Process::isStillAlive() {
    // Attempting to do anything on a dead process will result in error.
    // Thus, if we could access the process status before, and we no longer can, it's dead.
    int fd = openat(static_cast<int>(handle), "status", O_RDONLY);
    if (fd < 0)
        return false;
    close(fd);
    return true;
}

bool LPA::Process::matchesNameTemplate(QString post) {
    int nm = openat(static_cast<int>(handle), "comm", O_RDONLY);
    if (nm < 0)
        return false;

    FILE * stdl = fdopen(nm, "rb");
    if (!stdl) {
        close(nm);
        return false;
    }
    bool answer;
    {
        QTextStream qts(stdl, QIODevice::ReadOnly);
        answer = post == qts.readLine();
    }
    fclose(stdl);
    return answer;
}

bool LPA::Process::canBeginMemoryAccess() {
    if (handle2 < 0)
        handle2 = openat(static_cast<int>(handle), "mem", O_RDWR);
    return handle2 >= 0;
}

void LPA::Process::readMemory(uint32_t address, void * res, size_t ressz) {
    if (handle2 >= 0) {
        lseek(handle2, address, SEEK_SET);
        read(handle2, res, ressz);
    }
}

void LPA::Process::writeMemory(uint32_t address, void * res, size_t ressz) {
    if (handle2 >= 0) {
        lseek(handle2, address, SEEK_SET);
        write(handle2, res, ressz);
    }
}

bool LPA::getProcesses(QList<intptr_t> & processes) {
    QDir qd("/proc");
    for (QString & qs : qd.entryList()) {
        bool ok = false;
        intptr_t r = static_cast<intptr_t>(qs.toLong(&ok));
        if (ok)
            processes.append(r);
    }
    return true;
}

unsigned int LPA::getLastError() {
    return static_cast<unsigned int>(errno);
}
