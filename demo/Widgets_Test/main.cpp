#include "Widgets_Test.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <qpushbutton.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widgets_Test w;

    // 信号、槽:点击Close按钮后关闭窗口
    QPushButton button("Close", &w);
    button.move(QPoint(500, 350));
    button.resize(50, 20);

    // Qt预定义的信号和槽
    QObject::connect(&button, &QPushButton::clicked, &w, &Widgets_Test::close);

    w.show();
    return a.exec();
}
