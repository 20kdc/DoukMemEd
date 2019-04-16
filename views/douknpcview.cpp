#include "doukutsu.h"
#include "douknpcview.h"
#include "ui_douknpcview.h"
#include <QDebug>

DoukNPCView::DoukNPCView(LPA::Process * proc) : DoukView(proc, "NPC Status"),
    ui(new Ui::DoukNPCView)
{
    currentNPC = 0;
    ui->setupUi(this);
    for (int i = 0; i < Doukutsu::NPCCount; i++)
        ui->listWidget->addItem("---");
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DoukNPCView::on_listWidget_currentRowChanged);
}

void DoukNPCView::timerRefresh()
{
    proc->readMemory(Doukutsu::NPCBase, npcBuffer, sizeof(Doukutsu::NPC) * Doukutsu::NPCCount);
    int npcCount = 0;
    for (int i = 0; i < Doukutsu::NPCCount; i++) {
        QString num = QString::number(i, 16);
        while (num.length() < 3)
            num = "0" + num;
        num = "NPC " + num;
        if (npcBuffer[i].inuse) {
            npcCount++;
            num += " : " + QString::number(npcBuffer[i].npcType) + " (#" + QString::number(npcBuffer[i].EventNum) + ")";
        }
        QListWidgetItem * lwi = ui->listWidget->item(i);
        if (lwi->text() != num)
            ui->listWidget->item(i)->setText(num);
    }
    QString qs = "";
    qs += "NPC " + QString::number(currentNPC, 16) + ", type " + QString::number(npcBuffer[currentNPC].npcType) + "\n";
    qs += "Xt: " + QString::number(npcBuffer[currentNPC].X >> 13) + "\n";
    qs += "Yt: " + QString::number(npcBuffer[currentNPC].Y >> 13) + "\n";
    qs += "Xv: " + QString::number(npcBuffer[currentNPC].movex) + "\n";
    qs += "Yv: " + QString::number(npcBuffer[currentNPC].movey) + "\n";
    qs += "Xf: " + QString::number(npcBuffer[currentNPC].X & 0x1FFF) + "\n";
    qs += "Yf: " + QString::number(npcBuffer[currentNPC].Y & 0x1FFF) + "\n";
    qs += "NPC Count: " + QString::number(npcCount) + "\n";
    ui->label->setText(qs);
}

void DoukNPCView::on_listWidget_currentRowChanged(int row) {
    if (row >= 0)
        currentNPC = row;
}

DoukNPCView::~DoukNPCView()
{
    delete ui;
}
