#include <string>
#include "lpa.h"

// Use for checking compilability. Do not commit with the 'wine' section active.

//#include <wine/windows/windows.h>
//#include <wine/windows/psapi.h>
//#include <wine/windows/winbase.h>

#include <Windows.h>
#include <Psapi.h>
#include <WinBase.h>

#include <iostream>
using namespace std;

LPA::Process::Process(QObject * parent, intptr_t pidx) : QObject(parent) {
    pid = pidx;
    handle = reinterpret_cast<intptr_t>(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, true, static_cast<DWORD>(pidx)));
    if (!handle) {
        // Handle was uncreatable, object is invalid.
        valid = false;
    } else {
        valid = true;
    }
}

LPA::Process::~Process() {
    if (valid)
        CloseHandle(reinterpret_cast<HANDLE>(handle));
}

bool LPA::Process::isStillAlive() {
    DWORD code = STILL_ACTIVE + 1; // In case of error, this ensures it's not assumed to be alive.
    GetExitCodeProcess(reinterpret_cast<void *>(handle), &code);
    return code == STILL_ACTIVE;
}

bool LPA::Process::matchesNameTemplate(QString post) {
    char name[513];
    // Must be kept in scope to ensure that the constData's conversion to an std::string occurs correctly.
    QByteArray qba = post.toUtf8();
    std::string doukutsu = qba.constData();
    DWORD ns = GetProcessImageFileNameA(reinterpret_cast<HANDLE>(handle), name, 512);
    cout << "PID: " << pid << ", GetProcessImageFileNameA returns: " << name << endl;
    if (ns != 0) {
        char* nameStart = strrchr(name, '\\');
        if (nameStart) {
            nameStart++;
        } else {
            nameStart = name;
        }
        return nameStart == doukutsu;
    }
    return false;
}

bool LPA::Process::beginMemoryAccessOrInvalidate() {
    return true;
}

void LPA::Process::readMemory(uint32_t address, void * res, size_t ressz) {
    ReadProcessMemory(reinterpret_cast<HANDLE>(handle), reinterpret_cast<LPVOID>(address), res, ressz, nullptr);
}

void LPA::Process::writeMemory(uint32_t address, void * res, size_t ressz) {
    WriteProcessMemory(reinterpret_cast<HANDLE>(handle), reinterpret_cast<LPVOID>(address), res, ressz, nullptr);
}

bool LPA::getProcesses(QList<intptr_t> & processes) {
    size_t power = 1024;
    while (true) {
        size_t total = sizeof(DWORD) * power;
        DWORD pids[power];
        DWORD szRet;
        if (!EnumProcesses(pids, total, &szRet))
            return false;
        if (szRet != total) {
            DWORD * ptr = pids;
            while (szRet) {
                processes.append(*ptr);
                ptr++;
                szRet -= sizeof(DWORD);
            }
            return true;
        }
        // More processes.
        power *= 2;
    }
}

unsigned int LPA::getLastError() {
    return GetLastError();
}
