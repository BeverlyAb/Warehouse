#ifndef PATHMAKER_H
#define PATHMAKER_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QMainWindow>


namespace Ui {
class PathMaker;
}

class PathMaker : public QDialog
{
    Q_OBJECT

public:
    explicit PathMaker(QWidget *parent = 0);
    ~PathMaker();
    void readFile(QString in, QVector<QPoint> &polyPoints);
    void Route();

private:
    Ui::PathMaker *ui;
protected:
    void paintEvent(QPaintEvent *e );
    QString dirToStr(const int & d);

    QVector<int> xPt;
    QVector<int> yPt;
};

#endif // PATHMAKER_H
