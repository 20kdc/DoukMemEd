#ifndef DOUKNPCVIEW_H
#define DOUKNPCVIEW_H

#include <QWidget>
#include <QListWidget>
#include "doukview.h"
#include "doukutsu.h"

namespace Ui {
class DoukNPCView;
}

class DoukNPCView : public DoukView
{
    Q_OBJECT

public:
    explicit DoukNPCView(LPA::Process * proc);
    virtual ~DoukNPCView() override;
    virtual void timerRefresh() override;
    int currentNPC;
private slots:
    void on_listWidget_currentRowChanged(int row);
private:
    Ui::DoukNPCView *ui;
    Doukutsu::NPC npcBuffer[Doukutsu::NPCCount];
};

#endif // DOUKNPCVIEW_H
