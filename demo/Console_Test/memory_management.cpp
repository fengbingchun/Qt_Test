#include "funset.hpp"
#include <QObject>
#include <QDebug>

// Blog: https://blog.csdn.net/fengbingchun/article/details/146456292

namespace {

class MyObject : public QObject {
public:
	explicit MyObject(const QString& name, QObject* parent = nullptr) : QObject(parent), name_(name)
	{
		qDebug() << "Constructor: " << name_;
	}

	~MyObject()
	{
		qDebug() << "Destructor: " << name_;
	}

	QString name() const
	{
		return name_;
	}

private:
	QString name_{};
};

} // namespace

int test_memory_management()
{
	MyObject* parent = new MyObject("parent object");
	
	MyObject* child1 = new MyObject("child object1", parent);

	//MyObject* child2 = new MyObject("child object2", parent);
	MyObject* child2 = new MyObject("child object2");
	child2->setParent(parent);

	for (auto obj : parent->children()) {
		qDebug() << "child object name: " << ((MyObject*)obj)->name();
	}

	qDebug() << "child2 parent object name: " << ((MyObject*)child2->parent())->name();

	delete parent; // manually delete the parent object
	return 0;
}
