#设置cmake的最低版本要求
cmake_minimum_required(VERSION 3.20)

#复制shader文件到构建目录
file(GLOB ASSETS "./assets" )
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

# 递归收集 assets 文件夹下所有文件
file(GLOB_RECURSE ASSET_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
# 在 VS 工程中显示 assets 文件夹
source_group("assets" FILES ${ASSET_FILES})

#设置c++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置项目名称和版本
project(QtGuiApp LANGUAGES CXX)

# 查找所需要的库
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets Quick QuickWidgets SerialPort)
# 调试信息：显示 Qt6 的路径
message(STATUS "Qt6 found at: ${Qt6_DIR}")
message(STATUS "Qt6 version: ${Qt6_VERSION}")
message(STATUS "Qt6 libraries: ${Qt6_LIBRARIES}")
find_package(VTK REQUIRED)
set (OpenCV_DIR "${CMAKE_SOURCE_DIR}/Packages/OpenCV4100")
find_package(OpenCV REQUIRED)
#GLM库
set (GLM_DIR "${CMAKE_SOURCE_DIR}/Packages/glm-1.0.1-light")

if(NOT VTK_FOUND)
    message(FATAL_ERROR "VTK not found!")
endif()

# 启用 AUTOMOC（QT关键！）
set(CMAKE_AUTOMOC ON)   # 自动运行 moc，处理 Q_OBJECT 宏，生成元对象代码（信号槽机制依赖）
set(CMAKE_AUTOUIC ON)   # 自动处理 .ui 文件，生成对应的界面代码
set(CMAKE_AUTORCC ON)   # 自动处理 .qrc 资源文件，生成资源访问代码

# 收集源文件（递归收集所有子文件夹中的头文件和源文件）
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "code/*.h"  
    "code/*.cpp"
    "control/*.h"
    "control/*.cpp"
    "control/*.png"
)

# 为每个文件创建对应的文件夹分组
foreach(FILE_PATH IN LISTS SRC_FILES)
    file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${FILE_PATH}")
    get_filename_component(FILE_DIR "${REL_PATH}" DIRECTORY)
    source_group("${FILE_DIR}" FILES "${FILE_PATH}")
endforeach()

qt_add_resources(RESOURCES resources.qrc)




# 创建可执行文件，根据构建类型动态设置 WIN32 选项
#add_executable(QtGuiApp WIN32 ${SRC_FILES} ${RESOURCES} res/icon/apprc.rc ${ASSET_FILES})  # Release 模式：无控制台
add_executable(QtGuiApp ${SRC_FILES} ${RESOURCES}  res/icon/apprc.rc ${ASSET_FILES})

# 添加预编译头支持
target_precompile_headers(QtGuiApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/code/pch.h")

# 库头文件目录
target_include_directories(QtGuiApp
    PRIVATE 
        ${VTK_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
        ${GLM_DIR}
)

# 库lib目录
target_link_directories(QtGuiApp
    PRIVATE
        "${CMAKE_SOURCE_DIR}/lib"
        ${OpenCV_LIB_DIR}
)

# 链接lib库
target_link_libraries(QtGuiApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::SerialPort
    VTK::GUISupportQt  
    VTK::RenderingOpenGL2
    VTK::InteractionStyle
    VTK::CommonCore
    VTK::RenderingImage     
    VTK::IOImage
    VTK::ViewsContext2D
    VTK::RenderingCore
    VTK::CommonColor
    VTK::CommonDataModel
    VTK::ImagingCore
    VTK::InteractionImage
    VTK::RenderingContextOpenGL2
    VTK::RenderingFreeType
    VTK::RenderingGL2PSOpenGL2
    VTK::RenderingVolumeOpenGL2
    VTK::RenderingAnnotation 
    VTK::RenderingVolume
    VTK::FiltersGeneral
    VTK::FiltersGeometry
    VTK::FiltersModeling
    ${OpenCV_LIBS}
)

# 设置 VTK 模块依赖（确保头文件正确处理）
if(TARGET VTK::moc)
    add_dependencies(QtGuiApp VTK::moc)
endif()

#使用 QT_NO_DEBUG_OUTPUT 宏禁止release模式下的调试输出
#if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
#    add_definitions(-DQT_NO_DEBUG_OUTPUT)
#endif()

# 复制 opencv_world*.dll 到可执行文件目录
add_custom_command(TARGET QtGuiApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${OpenCV_DIR}/x64/vc17/bin/opencv_world4100d.dll"
    "${OpenCV_DIR}/x64/vc17/bin/opencv_world4100.dll"
    $<TARGET_FILE_DIR:QtGuiApp>
)
