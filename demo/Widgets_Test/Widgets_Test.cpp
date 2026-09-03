#include "Widgets_Test.h"
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>
#include <opencv2/opencv.hpp>

Widgets_Test::Widgets_Test(QWidget *parent)
    : QMainWindow(parent)
{
    ui_.setupUi(this);

    // 菜单栏
    auto menu_url = ui_.menu_bar->addMenu("网址");
    auto act_csdn = menu_url->addAction("CSDN网址");
    menu_url->addSeparator(); // 分隔线
    connect(act_csdn, &QAction::triggered, this, [this] {
        QMessageBox::information(this, "CSDN网址", "https://blog.csdn.net/fengbingchun/", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    });

    auto act_github = menu_url->addAction("GitHub网址");
    connect(act_github, &QAction::triggered, this, [this] {
        QMessageBox::information(this, "GitHub网址", "https://github.com/fengbingchun", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    });

    auto menu_camera = ui_.menu_bar->addMenu("Camera");
    act_open_camera_ = menu_camera->addAction("Open Camera");
    connect(act_open_camera_, &QAction::triggered, this, &Widgets_Test::start_capture);
    menu_camera->addSeparator();
    act_close_camera_ = menu_camera->addAction("Close Camera");
    connect(act_close_camera_, &QAction::triggered, this, &Widgets_Test::close_caputre);

    act_open_camera_->setEnabled(true);
    act_close_camera_->setEnabled(false);

    connect(this, &Widgets_Test::camera_open_failed, this, [this]() {
        act_open_camera_->setEnabled(true);
        act_close_camera_->setEnabled(false);

        if (label_cap_) {
            label_cap_->deleteLater();
            label_cap_ = nullptr;
        }
    });

    // 工具栏
    ui_.main_tool_bar->addAction(act_csdn);
    ui_.main_tool_bar->addSeparator();
    ui_.main_tool_bar->addAction(act_github);
    ui_.main_tool_bar->addSeparator();

    ui_.main_tool_bar->addAction(act_open_camera_);
    ui_.main_tool_bar->addSeparator();
    ui_.main_tool_bar->addAction(act_close_camera_);
    ui_.main_tool_bar->addSeparator();

    // 禁止拖拽工具栏
    ui_.main_tool_bar->setMovable(false);
    ui_.main_tool_bar->setFloatable(false);
}

Widgets_Test::~Widgets_Test()
{
    close_caputre();
}

void Widgets_Test::start_capture()
{
    if (is_running_.load()) return;

    if (capture_thread_.joinable())
        capture_thread_.join();

    // Qt强制规则: QWidget以及派生类, 只能在主线程创建, 不能在别的线程new
    auto* central = ui_.central_widget;
    auto* main_layout = qobject_cast<QVBoxLayout*>(central->layout());
    if (!main_layout) {
        // Qt对象树规则: layout设置给某个widget之后，widget会接管这个layout的所有权; 当central_widget在窗口销毁阶段被释放时，Qt会自动delete这个layout
        main_layout = new QVBoxLayout(central);
        main_layout->setContentsMargins(2, 2, 2, 2); // 设置布局内边距(布局边缘和它内部子控件之间的空白距离,单位像素)
    }

    if (!label_cap_) {
        label_cap_ = new DisplayLabel(central);
        label_cap_->setStyleSheet("background-color:#000000;");
        label_cap_->setAlignment(Qt::AlignCenter);

        QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label_cap_->setSizePolicy(sp);
        label_cap_->setMinimumSize(1, 1);

        main_layout->addWidget(label_cap_, 1); // 把label_cap_控件添加到main_layout(QVBoxLayout)布局里面进行托管
        label_cap_->installEventFilter(this);
    }

    is_running_.store(true);
    capture_thread_ = std::thread([this] {
        this->capture();
    });

    act_open_camera_->setEnabled(false);
    act_close_camera_->setEnabled(true);
}

void Widgets_Test::capture()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        FBC_LOG_ERROR("failed to open camera"); // QMetaObject::invokeMethod, UI操作必须在主线程中执行
        is_running_.store(false);
        emit camera_open_failed();
        return;
    }

    cv::Mat frame{}, rgb{};
    QImage image{}, img_copy{};
    while (is_running_.load()) {
        cap >> frame;
        if (frame.empty())
            continue;

        cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
        image = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        img_copy = image.copy(); // 必须copy, Mat会被下一轮覆盖

        // 跨线程调用UI槽函数 QueuedConnection
        QMetaObject::invokeMethod(this, "update_frame", Qt::QueuedConnection, Q_ARG(QImage, img_copy));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Widgets_Test::update_frame(const QImage& image)
{
    if (!label_cap_) return;
    latest_frame_ = image;

    auto pix = QPixmap::fromImage(image).scaled(label_cap_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label_cap_->setPixmap(pix);
}

void Widgets_Test::close_caputre()
{
    if (label_cap_) {
        label_cap_->removeEventFilter(this);
        label_cap_->deleteLater();
        label_cap_ = nullptr;
    }

    is_running_.store(false);
    if (capture_thread_.joinable())
        capture_thread_.join();

    act_open_camera_->setEnabled(true);
    act_close_camera_->setEnabled(false);

    latest_frame_ = QImage();
}

bool Widgets_Test::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == label_cap_ && event->type() == QEvent::Resize) {
        if (!latest_frame_.isNull() && label_cap_) {
            QPixmap pix = QPixmap::fromImage(latest_frame_).scaled(label_cap_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            label_cap_->setPixmap(pix);
        }
    }

    return QMainWindow::eventFilter(watched, event);
}
