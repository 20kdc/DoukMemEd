#include "hkapplication.h"
#include "hkeventfilter.h"
#include "doukmemed.h"

int main(int argc, char *argv[])
{
    HKApplication a(argc, argv);
    a.installEventFilter(new HKEventFilter(a.instance()));

    DoukMemEd w;
    w.show();

    return a.exec();
}
