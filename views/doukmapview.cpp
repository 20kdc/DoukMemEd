#include <QSizePolicy>
#include <QPainter>
#include "doukmapview.h"

DoukMapView::DoukMapView(LPA::Process * pro) : DoukView(pro, "Player Position...")
{
    mapW = 0;
    mapH = 0;
    mapData = nullptr;
    // Disable this by default
    smeOfs = -1;
    setCursor(QCursor(Qt::CrossCursor));

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

void DoukMapView::timerRefresh() {
    uint16_t words[2];
    uint32_t ptr;
    words[0] = 0;
    words[1] = 0;
    proc->readMemory(Doukutsu::MapSize, words, sizeof(uint16_t) * 2);
    mapW = words[0];
    mapH = words[1];
    size_t mapsz = static_cast<size_t>(words[0]) * static_cast<size_t>(words[1]);

    if (mapData)
        delete[] mapData;
    mapData = nullptr;
    if (mapsz != 0) {
        mapData = new uint8_t[mapsz];
        proc->readMemory(Doukutsu::MapPtr, &ptr, sizeof(uint32_t));
        proc->readMemory(ptr, mapData, mapsz);
    }

    proc->readMemory(Doukutsu::MapPXA, mapPXA, 256);
    resize(mapW * 16, mapH * 16);
    repaint();
}

void DoukMapView::paintEvent(QPaintEvent * paint) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush whiteBrush(QColor(255, 255, 255));
    QBrush blueBrush(QColor(0, 0, 255));
    QBrush blackBrush(QColor(0, 0, 0));
    painter.fillRect(0, 0, mapW * 16, mapH * 16, blackBrush);

    int ofs = 0;
    for (int y = 0; y < mapH; y++) {
        for (int x = 0; x < mapW; x++) {
            int rx = x * 16;
            int ry = y * 16;
            //painter.drawText(x * 16, y * 16, QString("%1").arg((*subData) % 10));
            uint8_t v = mapPXA[mapData[ofs]];
            uint8_t vX = v & 15;
            uint8_t vY = v >> 4;
            if (vY & 2)
                painter.fillRect(rx, ry, 16, 16, blueBrush);
            int shape = 0;
            if (vY >= 4 && vY <= 7) {
                if (!(vY & 1)) {
                    if (vX == 1 || vX == 4 || vX >= 6)
                        shape = 1;
                } else {
                    shape = 1;
                    if (vX < 8)
                        shape = vX + 2;
                }
            }
            QPainterPath quip;
            quip.moveTo(0, 0);
            if (shape == 1) {
                quip.lineTo(16, 0);
                quip.lineTo(16, 16);
                quip.lineTo(0, 16);
            } else if (shape == 2) {
                // 0
                quip.lineTo(16, 0);
                quip.lineTo(16, 8);
                quip.lineTo(0, 16);
            } else if (shape == 3) {
                // 1
                quip.lineTo(16, 0);
                quip.lineTo(0, 8);
            } else if (shape == 4) {
                // 2
                quip.lineTo(16, 0);
                quip.lineTo(16, 8);
                quip.lineTo(0, 0);
            } else if (shape == 5) {
                // 3
                quip.lineTo(16, 0);
                quip.lineTo(16, 16);
                quip.lineTo(0, 8);
            } else if (shape == 6) {
                // 4
                quip.lineTo(16, 8);
                quip.lineTo(16, 16);
                quip.lineTo(0, 16);
            } else if (shape == 7) {
                quip.moveTo(0, 16);
                // 5
                quip.lineTo(0, 8);
                quip.lineTo(16, 16);
            } else if (shape == 8) {
                quip.moveTo(0, 16);
                // 6
                quip.lineTo(16, 8);
                quip.lineTo(16, 16);
            } else if (shape == 9) {
                quip.moveTo(0, 16);
                // 7
                quip.lineTo(0, 8);
                quip.lineTo(16, 0);
                quip.lineTo(16, 16);
            }
            quip.translate(rx, ry);
            painter.fillPath(quip, whiteBrush);
            //if (vY == 8 || vY == 10) {
            // arrows?
            //}
            if (smeOfs == ofs) {
                painter.setPen(QColor(64, 255, 64));
                QFont f = QFont("", 7);
                f.setStyleStrategy(QFont::NoAntialias);
                f.setOverline(false);
                painter.setFont(f);
                painter.fillRect(rx, ry, 16, 7, blackBrush);
                painter.drawText(rx + 1, ry + 7, QString("%0").arg(mapData[ofs]));
                painter.setPen(Qt::NoPen);
            }
            ofs++;
        }
    }

    int32_t playerX, playerY;

    proc->readMemory(Doukutsu::PlayerX, &playerX, sizeof(int32_t));
    proc->readMemory(Doukutsu::PlayerY, &playerY, sizeof(int32_t));

    playerX += 8 * 512;
    playerY += 8 * 512;

    QPainterPath playerSymPath;
    playerSymPath.moveTo(-8, 0);
    playerSymPath.lineTo(0, -8);
    playerSymPath.lineTo(8, 0);
    playerSymPath.lineTo(0, 8);
    playerSymPath.lineTo(-8, 0);
    playerSymPath.translate(playerX / 512, playerY / 512);
    painter.setPen(QColor(255, 0, 0));
    painter.drawPath(playerSymPath);

    painter.end();
    paint->accept();
}

void DoukMapView::mousePressEvent(QMouseEvent * mouse)
{
    Qt::MouseButton mb = mouse->button();
    if (mb == Qt::MouseButton::LeftButton) {
        int32_t playerX = (mouse->x() - 8) * 512;
        int32_t playerY = (mouse->y() - 8) * 512;
        proc->writeMemory(Doukutsu::PlayerX, &playerX, sizeof(int32_t));
        proc->writeMemory(Doukutsu::PlayerY, &playerY, sizeof(int32_t));
    }
    mouse->accept();
}
void DoukMapView::wheelEvent(QWheelEvent * wheel)
{
    if (!(mapW && mapH))
        return;
    int x = wheel->x() / 16;
    int y = wheel->y() / 16;
    if (x < 0)
        x = 0;
    if (x >= mapW)
        x = mapW - 1;
    if (y < 0)
        y = 0;
    if (y >= mapH)
        y = mapH - 1;

    uint16_t words[2];
    uint32_t ptr;
    words[0] = 0;
    words[1] = 0;
    proc->readMemory(Doukutsu::MapSize, words, sizeof(uint16_t) * 2);
    if ((words[0] != mapW) || (words[1] != mapH))
        return;
    int ofs = x + (y * mapW);
    smeOfs = ofs;
    int delta = wheel->delta();
    if (delta > 0)
        delta = 1;
    if (delta < 0)
        delta = -1;
    mapData[ofs] += delta;
    proc->readMemory(Doukutsu::MapPtr, &ptr, sizeof(uint32_t));
    proc->writeMemory(ptr + ofs, mapData + ofs, 1);
    wheel->accept();
}

DoukMapView::~DoukMapView()
{
    if (mapData)
        delete[] mapData;
}
