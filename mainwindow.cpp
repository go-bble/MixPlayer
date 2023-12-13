#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"
#include <iostream>
extern "C"{
    #include "libavformat/avformat.h"
}
#include <opencv2/opencv.hpp>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<avformat_configuration();
    std::cout << "OpenCV Version: " << cv::getVersionString() << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}
