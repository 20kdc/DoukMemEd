#include "hkapplication.h"

HKApplication::HKApplication(int& argc, char** argv) : QApplication(argc, argv)
{
}

bool HKApplication::isHotkeyRegistered(int k) {
    return hotkeyStates.contains(k);
}

bool HKApplication::registerHotkey(int k) {
    if (isHotkeyRegistered(k))
        return false;
    hotkeyStates.insert(k, false);
    return true;
}

bool HKApplication::unregisterHotkey(int k) {
    if (!isHotkeyRegistered(k))
        return false;
    hotkeyStates.remove(k);
    return true;
}

void HKApplication::notifyHotkeyStatus(int k, bool v, Qt::KeyboardModifiers m) {
    if (!isHotkeyRegistered(k))
        return;
    hotkeyStates[k] = v ? true : false;
    emit hotkey(k, v, m);
}
