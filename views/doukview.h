#ifndef DOUKVIEW_H
#define DOUKVIEW_H

#include <QWidget>
#include "lpa/lpa.h"

/*
 * This represents a view into (and possibly way to modify) game data.
 * All views are created upon attach, and are invalid upon detach.
 * It is the DoukMemEd's responsibility to delete all views before the LPA process object.
 */
class DoukView : public QWidget
{
    Q_OBJECT
public:
    explicit DoukView(LPA::Process * proc, const QString & buttonName);
    virtual void timerRefresh() = 0;
    const QString buttonName;
    LPA::Process * proc;

signals:

public slots:
};

#endif // DOUKVIEW_H
