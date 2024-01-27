#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    // 从C++加载QML对象
    QQmlApplicationEngine engine;
#ifdef CMAKE_BUILD_METHOD
    engine.load(QUrl(QStringLiteral("main.qml"))); // 使用CMakeLists.txt生成工程时需要使用此句
#else
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#endif
    if (engine.rootObjects().isEmpty())
        return -1;

    fprintf(stdout, "test finish\n");
    return app.exec();
}
