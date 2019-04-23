// Loaded Process Access (LPA)
#pragma once

#include <QList>
#include <stdint.h>

namespace LPA {
    class Process : public QObject
    {
        Q_OBJECT
    public:
        Process(QObject * parent, intptr_t pid);
        inline bool isValid() {
            return valid;
        }
        inline intptr_t getPID() {
            return pid;
        }
        bool isStillAlive();
        bool matchesNameTemplate(QString name);
        // Note: It's best if the Lea implementation *doesn't* open with full permissions unless necessary, but the Windows impl. was ported.
        // Still: Only do this if you're sure this is the process you want. Don't want to trigger anticheats.
        // Note that if this fails, the process is invalidated, so that's been made clear in the new function name (...sorry Leo)
        // Returns true on success.
        bool beginMemoryAccessOrInvalidate();
        void readMemory(uint32_t address, void * res, size_t ressz);
        void writeMemory(uint32_t address, void * res, size_t ressz);
        ~Process();
    private:
        intptr_t pid;
        // Implementation-specific contents.
        intptr_t handle, handle2;
        // Set during constructor.
        bool valid;
    };
    bool getProcesses(QList<intptr_t> & processes);
    unsigned int getLastError();
}
