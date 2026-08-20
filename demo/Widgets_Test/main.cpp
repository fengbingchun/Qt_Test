#include "widgets_tesT.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
#ifdef FBC_LOGGER
    fbc::Logger::get_instance().init("logs", "widgets_test", true, 10);
#endif

    QApplication a(argc, argv);

    // Blog: https://blog.csdn.net/fengbingchun/article/details/151020879
    QFile style_file(":/styles/style.qss");
    if (style_file.open(QFile::ReadOnly)) {
        QTextStream stream(&style_file);
        QString style_content = stream.readAll();
        FBC_LOG_INFO("qss length: {}", static_cast<long long>(style_content.length()));
        a.setStyleSheet(style_content);
        style_file.close();
    }
    else
        FBC_LOG_ERROR("failed to open style sheet file: {}", style_file.fileName().toStdString()); 

    Widgets_Test w;

    w.show();
    return a.exec();
}
