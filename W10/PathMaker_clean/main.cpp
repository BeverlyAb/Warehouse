#include "pathmaker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PathMaker w;

    QString  in = "/Users/fullstackmachine/Desktop/BevCode/AdvAlg_221/W10/pathOnly.txt";
    QVector<QPoint> b;
    //need to call readFile once more just for Route; class vectors did not carry over
    w.readFile(in, b);
    w.Route();

    w.show();

    return a.exec();
}
