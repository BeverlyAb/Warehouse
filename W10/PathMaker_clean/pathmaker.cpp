#include "pathmaker.h"
#include "ui_pathmaker.h"

PathMaker::PathMaker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PathMaker)
{
    ui->setupUi(this);
}

PathMaker::~PathMaker()
{
    delete ui;
}
void PathMaker::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QVector<QPoint> shelfPt;
    QVector<QPoint> pathPt;

    QString in = "";
    //make a pen
    QPen shelfpen;
    shelfpen.setWidth(10);
    shelfpen.setColor(Qt::red);
    painter.setPen(shelfpen);

    //shelf
    in = "/Users/fullstackmachine/Desktop/BevCode/AdvAlg_221/W10/shelf.txt";
    readFile(in, shelfPt);
    painter.drawPoints( shelfPt );

//    //path
    in = "/Users/fullstackmachine/Desktop/BevCode/AdvAlg_221/W10/pathOnly.txt";
    QPen linepen;
    linepen.setWidth(10);
    linepen.setColor(Qt::blue);

    painter.setPen(linepen);
    readFile(in, pathPt);
    painter.drawPoints( pathPt );
}

void PathMaker::readFile(QString in, QVector<QPoint> & polyPoints)
{
    QFile myFile(in);
    if(!myFile.open(QIODevice::ReadOnly))
             QMessageBox::information(0,"info", myFile.errorString());
    QTextStream whole(&myFile);

    QString str = whole.readLine();
    while(!whole.atEnd()){
        QStringList list = str.split(' ');
        int x = list.at(0).toInt()*12+10;
        int y = list.at(1).toInt()*12+10;
        polyPoints << QPoint(x,y);
        str = whole.readLine();
    }
    myFile.close();
}

void PathMaker::Route()
{

    QMessageBox::information(this, "succotash", "molasses");
}
