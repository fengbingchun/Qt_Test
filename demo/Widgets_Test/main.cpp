#include "widgets_tesT.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widgets_Test w;

    w.show();
    return a.exec();
}
