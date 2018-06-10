#include "pathmaker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PathMaker w;
    //w.readFile();
    w.show();

    return a.exec();
}
