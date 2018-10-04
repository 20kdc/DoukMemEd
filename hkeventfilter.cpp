#include "hkeventfilter.h"
#include "hkapplication.h"
#include <QKeyEvent>
#include <iostream>
using namespace std;

HKEventFilter::HKEventFilter(QObject *parent) : QObject(parent)
{
}

bool HKEventFilter::eventFilter(QObject* watched, QEvent* event) {
    // if watched object is not HKApplication, skip
    HKApplication* app = static_cast<HKApplication*>(watched);
    if (app == nullptr)
        return false;
    // if event is not KeyPress or KeyRelease, skip
    QEvent::Type evType = event->type();
    if (evType != QEvent::KeyPress && evType != QEvent::KeyRelease)
        return false;
    QKeyEvent* evKey = static_cast<QKeyEvent*>(event);
    // check if key is a registered hotkey
    int k = evKey->key();
    if (!app->isHotkeyRegistered(k))
        return false;
    // send signal
    Qt::KeyboardModifiers m = evKey->modifiers();
    bool curState = app->hotkeyStates[k];
    switch (evType) {
    case QEvent::KeyPress:
        if (!curState) {
            app->notifyHotkeyStatus(k, true, m);
            return true;
        }
        break;
    default:
        if (curState) {
            app->notifyHotkeyStatus(k, false, m);
            return true;
        }
        break;
    }
    return false;
}
