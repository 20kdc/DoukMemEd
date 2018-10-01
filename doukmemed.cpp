#include "doukmemed.h"
#include "ui_doukmemed.h"

DoukMemEd::DoukMemEd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DoukMemEd)
{
    ui->setupUi(this);
}

DoukMemEd::~DoukMemEd()
{
    delete ui;
}
