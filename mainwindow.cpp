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
