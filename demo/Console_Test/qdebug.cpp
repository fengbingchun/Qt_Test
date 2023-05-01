#include "qdebug.hpp"
#include <QDebug>
#include <QDate>
#include <QRect>

// Blog: https://blog.csdn.net/fengbingchun/article/details/130454199

namespace {

typedef struct {
	long x, y, z;
} Coordinate;

QDebug operator<<(QDebug debug, const Coordinate& c)
{
	// QDebugStateSaver limits changes to the formatting to the current scope
	QDebugStateSaver saver(debug);
	debug.nospace() << '(' << c.x << ", " << c.y << ", " << c.z << ')';

	return debug;
}

} // namespace

int test_qdebug_1()
{
	// qDebug(const char *message, ...):与C的printf(const char * format, ...)函数类似
	qDebug("current date: %d:%d:%d", QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day());
	printf("current date: %d:%d:%d\n", QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day());

	qDebug() << "Date:" << QDate::currentDate();

	QString s("beijing");
	qDebug() << "s:" << s;

	QByteArray ba("haidian");
	qDebug() << "ba:" << ba;

	// 注意以下两条语句输出的差异
	qDebug() << "Types:" << QString("String") << QChar('x') << QRect(0, 10, 50, 40);
	qDebug().nospace().noquote() << "Types:" << QString("String") << QChar('x') << QRect(0, 10, 50, 40);

	// 将自定义类型写入流
	Coordinate c = { 10, 20, 30 };
	qDebug() << "Coordinate:" << c;

	return 0;
}
