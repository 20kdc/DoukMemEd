#ifndef DOUKMAPVIEW_H
#define DOUKMAPVIEW_H

#include <QWidget>
#include "doukmemed.h"

class DoukMapView : public QWidget
{
    Q_OBJECT

public:
    explicit DoukMapView(DoukMemEd * target, LPA::Process * proc);
    ~DoukMapView();

    virtual void paintEvent(QPaintEvent * paint) override;
    virtual void mousePressEvent(QMouseEvent * mouse) override;
    virtual void wheelEvent(QWheelEvent * wheel) override;

private slots:
    void detach();
    void timerRefresh();
private:
    // Cache for between timer refreshes
    int mapW, mapH;
    uint8_t * mapData;
    uint8_t mapPXA[256];

    int smeOfs;

    LPA::Process *proc;
};

#endif // DOUKMAPVIEW_H
