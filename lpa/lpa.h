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
        // WARNING (for Windows): If this method returns false, this object is no longer valid!
        bool tryBeginMemoryAccess();
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
