#include "widgets_tesT.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Blog: https://blog.csdn.net/fengbingchun/article/details/151020879
    QFile style_file(":/styles/style.qss");
    if (style_file.open(QFile::ReadOnly)) {
        QTextStream stream(&style_file);
        QString style_content = stream.readAll();
        qDebug() << "qss length: " << style_content.length();
        a.setStyleSheet(style_content);
        style_file.close();
    }
    else
        qDebug() << "failed to open style sheet file";

    Widgets_Test w;

    w.show();
    return a.exec();
}
