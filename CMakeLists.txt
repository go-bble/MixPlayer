cmake_minimum_required(VERSION 3.5)

project(mixplayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

#---


# set(VCPKG_BIN_DIR "D:/workspace/vcpkg/vcpkg")
set(VCPKG_BIN_DIR "G:/workspace-G/vcpkg/vcpkg")
set(VCPKG_ROOT "${VCPKG_BIN_DIR}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_INSTALLED_DIR "${VCPKG_BIN_DIR}/installed")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_BIN_DIR}/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_MODULE_PATH "${VCPKG_INSTALLED_DIR}/x64-windows/share/ffmpeg")
# list(APPEND  CMAKE_PREFIX_PATH  "G:/workspace-G/vcpkg/vcpkg/installed/x64-windows/share/ffmpeg"
#             "G:/workspace-G/vcpkg/vcpkg/installed/x64-windows/share/opencv4"
# )

find_package(FFMPEG REQUIRED)
if(FFMPEG_FOUND)
    message("++FFMPEG_FOUND++")
    message("FFMPEG_INCLUDE_DIRS: ${FFMPEG_INCLUDE_DIRS}")
    message("FFMPEG_LIBRARY_DIRS: ${FFMPEG_LIBRARY_DIRS}")
    message("FFMPEG_LIBRARIES: ${FFMPEG_LIBRARIES}")
    message("FFMPEG_VERSION: ${FFMPEG_VERSION}")
else()
    message("--FFMPEG_FOUND--")
endif()

set(VCPKG_LIBRARY_DIRS
    "${VCPKG_INSTALLED_DIR}/x64-windows/lib"
    "${VCPKG_INSTALLED_DIR}/x64-windows/debug/lib"
)
set(OpenCV_LIBRARIES)
foreach(lib_dir_opencv ${VCPKG_LIBRARY_DIRS})
    file(GLOB opencv_libs "${lib_dir_opencv}/opencv*.lib")
    list(APPEND OpenCV_LIBRARIES ${opencv_libs})
endforeach()
message("OpenCV libraries: ${OpenCV_LIBRARIES}")
message("QT_DIR libraries: ${QT_DIR}")
#---

set(TS_FILES mixplayer_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mixplayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET mixplayer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(mixplayer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(mixplayer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
#----------
target_include_directories(mixplayer PRIVATE
    ${FFMPEG_INCLUDE_DIRS}
)
target_link_directories(mixplayer PRIVATE
    ${FFMPEG_LIBRARY_DIRS}
)
target_link_libraries(mixplayer PRIVATE
    ${FFMPEG_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${lib_path}

)


#-----------
target_link_libraries(mixplayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.mixplayer)
endif()
set_target_properties(mixplayer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS mixplayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mixplayer)
endif()
