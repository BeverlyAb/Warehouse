#include "pathmaker.h"
#include "ui_pathmaker.h"

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

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

    if(in == "/Users/fullstackmachine/Desktop/BevCode/AdvAlg_221/W10/pathOnly.txt"){
        //make sure nothing is appended
        int n = xPt.size();
        for(int i = 0; i < n; i++){
            xPt.pop_back();
            yPt.pop_back();
        }
    }
    while(!whole.atEnd()){
        QStringList list = str.split(' ');
        int x = list.at(0).toInt();
        int y = list.at(1).toInt();

        if(in == "/Users/fullstackmachine/Desktop/BevCode/AdvAlg_221/W10/pathOnly.txt"){
            xPt.push_back(x);
            yPt.push_back(y);
        }

        //scaling to screen
        x = x * 12+10;
        y = y * 12 +10;
        polyPoints << QPoint(x,y);
        str = whole.readLine();
    }
    myFile.close();
}

void PathMaker::Route()
{
    QString size= QString::number(xPt.size());

    int hop = 1; //count init pos.
    int dir = UP;
    int prevDir = UP;
    QString term = "";

    for(int i = 0; i < xPt.size()-1 ; i++){
       // printf("x %i, y %i\n", xPt[i], yPt[i]);

        if(xPt[i] != xPt[i + 1]){ //left or right
            if(xPt[i] < xPt[i + 1])
                dir = RIGHT;
            else
                dir = LEFT;

        } else if (yPt[i] != yPt[i + 1]){// traveled up or down
            if(yPt[i] < yPt[i + 1])
                dir = DOWN;
            else
                dir = UP;
        }

        if(i == 0)
            prevDir = dir;

        else if(!(xPt[i] == xPt[i+1] && yPt[i] == yPt[i + 1])){
            if(dir == prevDir)
                hop++;
            else{
                term += dirToStr(prevDir) + QString::number(hop) + "\n";
                hop = 1;
                prevDir = dir;
            }
        }
    }
    //get last turn
    term += dirToStr(prevDir) + QString::number(hop) + "\n";

    QMessageBox::information(this, "boxy", term);
}
QString PathMaker::dirToStr(const int & d)
{
    if(d == LEFT)
        return "LEFT ";
    else if(d == RIGHT)
        return "RIGHT ";
    else if(d == UP)
        return "UP ";
    else if(d == DOWN)
        return "DOWN ";
    else
        return "whoops ";
}

