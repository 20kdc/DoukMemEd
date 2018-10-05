#include <QApplication>
#include "doukmemed.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DoukMemEd w;
    w.show();

    return a.exec();
}
