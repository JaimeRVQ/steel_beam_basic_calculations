#include "beams_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BeamsWidget w;
    w.show();

    return a.exec();
}
