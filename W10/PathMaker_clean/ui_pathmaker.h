/********************************************************************************
** Form generated from reading UI file 'pathmaker.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATHMAKER_H
#define UI_PATHMAKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_PathMaker
{
public:
    QPushButton *Route;

    void setupUi(QDialog *PathMaker)
    {
        if (PathMaker->objectName().isEmpty())
            PathMaker->setObjectName(QStringLiteral("PathMaker"));
        PathMaker->resize(727, 492);
        Route = new QPushButton(PathMaker);
        Route->setObjectName(QStringLiteral("Route"));
        Route->setGeometry(QRect(330, 240, 113, 32));

        retranslateUi(PathMaker);

        QMetaObject::connectSlotsByName(PathMaker);
    } // setupUi

    void retranslateUi(QDialog *PathMaker)
    {
        PathMaker->setWindowTitle(QApplication::translate("PathMaker", "PathMaker", nullptr));
        Route->setText(QApplication::translate("PathMaker", "Route", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PathMaker: public Ui_PathMaker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATHMAKER_H
