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


 set(VCPKG_BIN_DIR "D:/workspace/vcpkg/vcpkg")  #test
#set(VCPKG_BIN_DIR "G:/workspace-G/vcpkg/vcpkg") #home
set(VCPKG_ROOT "${VCPKG_BIN_DIR}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_INSTALLED_DIR "${VCPKG_BIN_DIR}/installed")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_BIN_DIR}/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_MODULE_PATH "${VCPKG_INSTALLED_DIR}/x64-windows/share/ffmpeg")
# list(APPEND  CMAKE_PREFIX_PATH  "G:/workspace-G/vcpkg/vcpkg/installed/x64-windows/share/ffmpeg"
#             "G:/workspace-G/vcpkg/vcpkg/installed/x64-windows/share/opencv4"
# )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug 模式
    set(VCPKG_LIBRARY_DIRS "${VCPKG_INSTALLED_DIR}/x64-windows/debug/lib" )
    set(VCPKG_DLL_DIRS "${VCPKG_INSTALLED_DIR}/x64-windows/debug/bin")
else()
    # Release 模式
    set(VCPKG_LIBRARY_DIRS "${VCPKG_INSTALLED_DIR}/x64-windows/lib")
    set(VCPKG_DLL_DIRS "${VCPKG_INSTALLED_DIR}/x64-windows/bin")
endif()



set(FFMPEG_DIR "")
find_package(FFMPEG REQUIRED)
if(FFMPEG_FOUND)
    message("++FFMPEG_FOUND++")
    message("FFMPEG_INCLUDE_DIRS: ${FFMPEG_INCLUDE_DIRS}")
    message("FFMPEG_LIBRARY_DIRS: ${FFMPEG_LIBRARY_DIRS}")
    message("FFMPEG_LIBRARIES: ${FFMPEG_LIBRARIES}")
    message("FFMPEG_DLLS: ${FFMPEG_DLLS}")
    message("FFMPEG_VERSION: ${FFMPEG_VERSION}")
else()
    message("--FFMPEG_FOUND--")
endif()





set(OpenCV_LIBRARIES)
foreach(lib_dir_opencv ${VCPKG_LIBRARY_DIRS})
    file(GLOB opencv_libs "${lib_dir_opencv}/opencv*.lib")
    list(APPEND OpenCV_LIBRARIES ${opencv_libs})                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
endforeach()



set(OpenVDB_INCLUDE_DIRS "C:/Program Files/OpenVDB/include")
set(OpenVDB_LIBRARIES)
foreach(lib_dir_openvdb "C:/Program Files/OpenVDB/lib")
    file(GLOB openvdb_libs "${lib_dir_openvdb}/openvd*.lib")
    list(APPEND OpenVDB_LIBRARIES ${openvdb_libs})
endforeach()
message("OpenVDB libraries: ${OpenVDB_LIBRARIES}")

set(jsoncpp_DIR "${VCPKG_BIN_DIR}/packages/jsoncpp_x64-windows/share/jsoncpp")
find_package(jsoncpp CONFIG REQUIRED)
set(jsoncpp_LIBRARIES)
foreach(lib_dir_jsoncpp ${VCPKG_LIBRARY_DIRS})
    file(GLOB jsoncpp_libs "${lib_dir_jsoncpp}/jsoncpp*.lib")
    list(APPEND jsoncpp_LIBRARIES ${jsoncpp_libs})
endforeach()
    message("++jsoncpp_FOUND++")
    message("jsoncpp_INCLUDE_DIRS: ${jsoncpp_INCLUDE_DIRS}")
    message("jsoncpp_LIBRARY_DIRS: ${jsoncpp_LIBRARY_DIRS}")
    message("jsoncpp_LIBRARIES: ${jsoncpp_LIBRARIES}")
    message("jsoncpp_VERSION: ${jsoncpp_VERSION}")


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
    ${OpenVDB_INCLUDE_DIRS}
)
target_link_directories(mixplayer PRIVATE
    ${FFMPEG_LIBRARY_DIRS}
)

target_link_libraries(mixplayer PRIVATE
    ${FFMPEG_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${OpenVDB_LIBRARIES}
    JsonCpp::JsonCpp
    Qt${QT_VERSION_MAJOR}::Widgets
    ${lib_path}

)


#-----------

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


#--------------------------------复制qt6/vcpkg*.dll-------------------------
file(GLOB DLL_FILES ${VCPKG_DLL_DIRS}/*.dll)
file(COPY ${DLL_FILES} DESTINATION ${PROJECT_BINARY_DIR})


#* ↓↓↓ 输出信息
macro(Proj_SHOW_MSG)
	message(STATUS "\n\n\n")
	message(STATUS "=======================  输出配置: ${PROJECT_NAME} =========================")
	message(STATUS "CMAKE_CURRENT_LIST_LINE: ${CMAKE_CURRENT_LIST_LINE}") #当前行行号
	message(STATUS "工程名称: ${PROJECT_NAME}")
	message(STATUS "工程版本: ${PROJECT_VERSION}")
	message(STATUS "CMake 版本:		${CMAKE_VERSION}")
	message(STATUS "构建工具:		${CMAKE_GENERATOR}")
	message(STATUS "\n" )
	
	message(STATUS "C++编译器:		${CMAKE_CXX_COMPILER_ID}  ${CMAKE_CXX_COMPILER_VERSION}") #${CMAKE_CXX_COMPILER_ID}
	message(STATUS "C 编译器:		${CMAKE_C_COMPILER_ID}  ${CMAKE_C_COMPILER_VERSION}")
	message(STATUS "CUDA 编译器:   	${CMAKE_CUDA_COMPILER_ID}  ${CMAKE_CUDA_COMPILER_VERSION}")
	message(STATUS "\n" )

	
	message(STATUS "CMAKE_CONFIGURATION_TYPES: 	${CMAKE_CONFIGURATION_TYPES}")
	message(STATUS "CMAKE_BUILD_TYPE: 			${CMAKE_BUILD_TYPE}") #build还是release?
	message(STATUS "\n" )
	
	message(STATUS "PROJECT_SOURCE_DIR:		${PROJECT_SOURCE_DIR}") #* 当前工程 CMake 文件所在目录
	message(STATUS "PROJECT_BINARY_DIR:		${PROJECT_BINARY_DIR}") #* 当前工程 build 目录
	message(STATUS "CMAKE_SOURCE_DIR:		${CMAKE_SOURCE_DIR}") #* 顶层 CMake 文件所在目录(整个项目根目录)
	message(STATUS "CMAKE_BINARY_DIR:		${CMAKE_BINARY_DIR}") #* 顶层 CMake 的 build 目录
	message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}") # 正在处理的项目的 
	message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}") #* 当前 cmake 的 build 目录
	message(STATUS "CMAKE_CURRENT_LIST_DIR:	${CMAKE_CURRENT_LIST_DIR}") #* 当前 cmake 文件路径
	message(STATUS "CMAKE_MODULE_PATH:		${CMAKE_MODULE_PATH}" ) #CMake 模块的路径列表
	message(STATUS "\n" )

	message(STATUS "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}" ) #deprecated! 
	message(STATUS "LIBRARY_OUTPUT_PATH:    ${LIBRARY_OUTPUT_PATH}" )
	message(STATUS "\n" )
	
	message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY:		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")# 静态库lib  动态库符号lib(dll)
	message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY:		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")# MODULE库dll, 动态库so/dylib (windows 下不输出)
	message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY:		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")# 可执行exe SHARED动态库dll
	message(STATUS "CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY:	${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}")
	message(STATUS "CMAKE_PDB_OUTPUT_DIRECTORY:			${CMAKE_PDB_OUTPUT_DIRECTORY}")
	message(STATUS "\n" )
	message("flags: ${CMAKE_CXX_FLAGS}")
	message("flags_debug: ${CMAKE_CXX_FLAGS_DEBUG}")
	message("flags_release: ${CMAKE_CXX_FLAGS_RELEASE}")
	
	message(STATUS "===========  ${PROJECT_NAME}: install 配置 ==============")
	message(STATUS "Install Destination: 	${CMAKE_INSTALL_PREFIX}") 
	message(STATUS "Install LibDir:			${CMAKE_INSTALL_LIBDIR}") 
	message(STATUS "==============  ${PROJECT_NAME}: 打印完成      =================\n\n" )
endmacro()
Proj_SHOW_MSG()