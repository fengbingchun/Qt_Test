#include "Widgets_Test.h"
#include <string>
#include <qdebug.h>
#include <qmessagebox.h>

Widgets_Test::Widgets_Test(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //connect(this, SIGNAL(get_address(QString)), this, SLOT(set_address(QString))); // 不推荐使用SIGNAL, SLOT
    connect(this, &Widgets_Test::get_address, this, &Widgets_Test::set_address);
}

Widgets_Test::~Widgets_Test()
{}

void Widgets_Test::on_csdn_address_clicked()
{
    emit get_address("csdn");
    //get_address("csdn"); // 可以省略emit
}

void Widgets_Test::on_github_address_clicked()
{
    emit get_address("github");
}

void Widgets_Test::set_address(const QString& str)
{
    // 试试std::string到QString的转换
    const static std::string csdn = "https://blog.csdn.net/fengbingchun/", github = "https://github.com/fengbingchun";

    QString ret = "^_^";
    if (str == "csdn") {
        ret = QString::fromStdString(csdn);
    } else if (str == "github") {
        ret = QString::fromStdString(github);
    }

    //qDebug() << ret;
    QMessageBox::information(nullptr, "Result", ret, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}