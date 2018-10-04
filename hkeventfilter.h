#pragma once

#include <QObject>

class HKEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit HKEventFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
};
