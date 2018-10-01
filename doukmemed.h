#ifndef DOUKMEMED_H
#define DOUKMEMED_H

#include <QMainWindow>

namespace Ui {
class DoukMemEd;
}

class DoukMemEd : public QMainWindow
{
    Q_OBJECT

public:
    explicit DoukMemEd(QWidget *parent = nullptr);
    ~DoukMemEd();

private:
    Ui::DoukMemEd *ui;
};

#endif // DOUKMEMED_H
