#include "Widgets_Test.h"
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

Widgets_Test::Widgets_Test(QWidget *parent)
    : QMainWindow(parent)
{
    ui_.setupUi(this);

    // 菜单栏
    auto menu_project = ui_.menu_bar->addMenu("网址");
    auto act_csdn = menu_project->addAction("CSDN网址");
    menu_project->addSeparator(); // 分隔线
    connect(act_csdn, &QAction::triggered, this, [this] {
        QMessageBox::information(this, "CSDN网址", "https://blog.csdn.net/fengbingchun/", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    });

    auto act_github = menu_project->addAction("GitHub网址");
    connect(act_github, &QAction::triggered, this, [this] {
        QMessageBox::information(this, "GitHub网址", "https://github.com/fengbingchun", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    });

    // 工具栏
    ui_.main_tool_bar->addAction(act_csdn);
    ui_.main_tool_bar->addSeparator();
    ui_.main_tool_bar->addAction(act_github);
    ui_.main_tool_bar->addSeparator();

    // 禁止拖拽工具栏
    ui_.main_tool_bar->setMovable(false);
    ui_.main_tool_bar->setFloatable(false);


    //connect(this, SIGNAL(get_address(QString)), this, SLOT(set_address(QString))); // 不推荐使用SIGNAL, SLOT
    connect(this, &Widgets_Test::get_address, this, &Widgets_Test::set_address);

    connect(ui_.button_close, &QPushButton::clicked, this, &Widgets_Test::close);
    connect(ui_.open_camera, &QPushButton::clicked, this, &Widgets_Test::start_capture);
    connect(ui_.close_camera, &QPushButton::clicked, this, & Widgets_Test::close_caputre);

    connect(ui_.open_camera, &QPushButton::clicked, this, [this] {
        ui_.button_close->setEnabled(false);
    });
    connect(ui_.close_camera, &QPushButton::clicked, this, [this] {
        ui_.button_close->setEnabled(true);
    });
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

void Widgets_Test::start_capture()
{
    is_running_.store(true);
    capture_thread_ = std::thread([this] {
        this->capture();
    });
}

void Widgets_Test::capture()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        qWarning("failed to open camera"); // QMetaObject::invokeMethod, UI操作必须在主线程中执行
        emit ui_.close_camera->clicked();
        return;
    }

    auto w1 = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    auto h1 = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    auto w2 = ui_.label_show->width();
    auto h2 = ui_.label_show->height();
    qDebug() << w1 << "," << h1 << "," << w2 << "," << h2;

    cv::Mat frame{}, rgb{};
    QImage image{}, img_copy{};
    while (is_running_.load()) {
        cap >> frame;
        if (frame.empty())
            continue;

        cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
        cv::resize(rgb, rgb, cv::Size(w2, h2));
        image = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        img_copy = image.copy();
        QMetaObject::invokeMethod(this, "update_frame", Qt::QueuedConnection, Q_ARG(QImage, img_copy));

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Widgets_Test::update_frame(const QImage& image)
{
    ui_.label_show->setPixmap(QPixmap::fromImage(image));
}

void Widgets_Test::close_caputre()
{
    is_running_.store(false);
    if (capture_thread_.joinable())
        capture_thread_.join();
}
