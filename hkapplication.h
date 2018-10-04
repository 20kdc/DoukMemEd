#pragma once

#include <QApplication>

class HKApplication : public QApplication
{
    Q_OBJECT

public:
    QMap<int, bool> hotkeyStates;
    explicit HKApplication(int& argc, char** argv);
    bool registerHotkey(int k);
    bool unregisterHotkey(int k);
    bool isHotkeyRegistered(int k);
    void notifyHotkeyStatus(int k, bool v, Qt::KeyboardModifiers m);

signals:
    void hotkey(int, bool, Qt::KeyboardModifiers);
};
