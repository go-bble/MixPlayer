#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"
#include <iostream>
#include <fstream>
extern "C"{
    #include "libavformat/avformat.h"
}
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include <QFileDialog>

#include <json/json.h>

#ifdef foreach
#undef foreach
#endif

#ifndef Q_MOC_RUN
#if defined(emit)
#undef emit
#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>
#define emit // restore the macro definition of "emit", as it was defined in gtmetamacros.h
#else
#include <tbb/tbb.h>
#endif // defined(emit)
#endif // Q_MOC_RUN



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<avformat_configuration();
    std::cout << "OpenCV Version: " << CV_VERSION << std::endl;

    // 创建 JSON 对象
    Json::Value root;

    // 添加国家
    root["country"] = "中国";

    // 添加省级信息
    Json::Value guangdong;
    guangdong["province"] = "广东省";

    // 添加市级信息
    Json::Value guangzhou;
    guangzhou["city"] = "广州市";

    // 添加区级信息
    Json::Value tianhe;
    tianhe["district"] = "天河区";

    // 添加村级信息
    Json::Value village;
    village["village"] = "村名";

    // 将各级信息组织起来
    tianhe["villages"].append(village);
    guangzhou["districts"].append(tianhe);
    guangdong["cities"].append(guangzhou);

    // 将省级信息添加到根节点
    root["provinces"].append(guangdong);

    // 将 JSON 对象写入文件
    std::ofstream outfile("geography.json");

    // 使用 Json::StreamWriterBuilder 配置 Json::StreamWriter
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["indentation"] = "\t";  // 可选，添加缩进以提高可读性

    // 将 JSON 对象写入文件
    std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
    writer->write(root, &outfile);

    outfile.close();

    std::cout << "JSON 文件已创建成功。" << std::endl;


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


        // Initialize the OpenVDB library. This must be called at least
        // once per program and may safely be called multiple times.
        openvdb::initialize();

        // Create an empty floating-point grid with background value 0.
        openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();

        // Get an accessor for coordinate-based access to voxels.
        openvdb::FloatGrid::Accessor accessor = grid->getAccessor();

        // Define a coordinate with large signed indices.
        openvdb::Coord xyz(1000, -200000000, 30000000);

        // Set the voxel value at (1000, -200000000, 30000000) to 1.
        accessor.setValue(xyz, 1.0);

        // Verify that the voxel value at (1000, -200000000, 30000000) is 1.
        std::cout << "Grid" << xyz << " = " << accessor.getValue(xyz) << std::endl;

        // Reset the coordinates to those of a different voxel.
        xyz.reset(1000, 200000000, -30000000);

        // Verify that the voxel value at (1000, 200000000, -30000000) is
        // the background value, 0.
        std::cout << "Grid" << xyz << " = " << accessor.getValue(xyz) << std::endl;

        // Set the voxel value at (1000, 200000000, -30000000) to 2.
        accessor.setValue(xyz, 2.0);

        // Set the voxels at the two extremes of the available coordinate space.
        accessor.setValue(openvdb::Coord::min(), 3.0f);
        accessor.setValue(openvdb::Coord::max(), 4.0f);





}

MainWindow::~MainWindow()
{
    delete ui;
}
