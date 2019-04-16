#ifndef DOUKMEMED_H
#define DOUKMEMED_H

#include <QMainWindow>
#include <QTimer>
#include <QCheckBox>
#include <QPushButton>
#include "doukutsu.h"
#include "views/doukview.h"
#include "lpa/lpa.h"

namespace Ui {
class DoukMemEd;
}

class DoukMemEd : public QMainWindow
{
    Q_OBJECT

public:
    explicit DoukMemEd(QWidget *parent = nullptr);
    ~DoukMemEd();

private slots:
    void on_btnAttach_clicked();
    void updateLocks();

private:
    Ui::DoukMemEd *ui;
    LPA::Process *proc = nullptr;
    QList<DoukView *> views;
    QList<QPushButton *> viewButtons;
    void detach();
    bool checkProcStillRunning();
    QTimer *lockUpdateTimer;
};

#endif
