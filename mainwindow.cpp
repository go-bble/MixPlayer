#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"
#include <iostream>
extern "C"{
    #include "libavformat/avformat.h"
}
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<avformat_configuration();
    std::cout << "OpenCV Version: " << CV_VERSION << std::endl;

    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open Image File", "", "Images (*.png *.jpg *.bmp)");

    // 检查用户是否取消了对话框
    if (filePath.isEmpty()) {
    }else{
        // 将QString转换为std::string
        std::string imagePath = filePath.toStdString();
        cv::Mat image = cv::imread(imagePath);
        // 检查图片是否成功加载
        if (image.empty()) {
            std::cerr << "Error: Unable to load image." << std::endl;
        }else{
            // 显示图片
            cv::imshow("OpenCV Test", image);
            cv::waitKey(0);
        }



    }

    // 创建 VideoCapture 对象，参数为摄像头索引（通常是 0）或视频文件路径
    cv::VideoCapture cap(1);  // 如果要打开视频文件，可以传递文件路径

    // 检查摄像头是否成功打开
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
    }else{
        // 获取摄像头的基本信息
        double fps = cap.get(cv::CAP_PROP_FPS);
        int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
        int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

        std::cout << "FPS: " << fps << std::endl;
        std::cout << "Resolution: " << width << " x " << height << std::endl;

        // 逐帧读取摄像头
        while (true) {
            // 读取一帧
            cv::Mat frame;
            cap >> frame;

            // 如果摄像头读取失败，退出循环
            if (frame.empty()) {
                std::cerr << "Error: Could not read frame." << std::endl;
                break;
            }

            // 在这里添加对每一帧的处理

            // 显示当前帧
            cv::imshow("Camera Feed", frame);

            // 按下 'q' 键退出循环
            if (cv::waitKey(30) == 'q') {
                break;
            }
        }

        // 关闭摄像头
        cap.release();
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}
