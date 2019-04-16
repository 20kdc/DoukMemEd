#ifndef DOUKMAPVIEW_H
#define DOUKMAPVIEW_H

#include <QWidget>
#include "doukmemed.h"
#include "views/doukview.h"

class DoukMapView : public DoukView
{
    Q_OBJECT

public:
    explicit DoukMapView(LPA::Process * proc);
    virtual ~DoukMapView() override;
    virtual void timerRefresh() override;

    virtual void paintEvent(QPaintEvent * paint) override;
    virtual void mousePressEvent(QMouseEvent * mouse) override;
    virtual void wheelEvent(QWheelEvent * wheel) override;

private:
    // Cache for between timer refreshes
    int mapW, mapH;
    uint8_t * mapData;
    uint8_t mapPXA[256];

    int smeOfs;
};

#endif // DOUKMAPVIEW_H
