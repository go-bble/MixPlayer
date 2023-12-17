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



}

MainWindow::~MainWindow()
{
    delete ui;
}
