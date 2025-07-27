
cmake_minimum_required(VERSION 3.20)

# 在 VS 工程中显示 assets 文件夹
file(GLOB ASSETS "./assets" )
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})
file(GLOB_RECURSE ASSET_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
message(STATUS "Assets folder path: ${CMAKE_CURRENT_SOURCE_DIR}/assets")
add_custom_target(AssetsInIDE SOURCES ${ASSET_FILES})
foreach(ASSET_FILE IN LISTS ASSET_FILES)
    file(RELATIVE_PATH ASSET_REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${ASSET_FILE}")
    get_filename_component(ASSET_GROUP "${ASSET_REL_PATH}" DIRECTORY)
    string(REPLACE "/" "\\" ASSET_GROUP "${ASSET_GROUP}")
    source_group("${ASSET_GROUP}" FILES "${ASSET_FILE}")
endforeach()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(QtGuiApp LANGUAGES CXX)

# 查找所需要的库
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets Quick QuickWidgets SerialPort)
message(STATUS "Qt6 found at: ${Qt6_DIR}")
message(STATUS "Qt6 version: ${Qt6_VERSION}")
message(STATUS "Qt6 libraries: ${Qt6_LIBRARIES}")
set (OpenCV_DIR "${CMAKE_SOURCE_DIR}/Packages/OpenCV4100")
find_package(OpenCV REQUIRED)
set (GLM_DIR "${CMAKE_SOURCE_DIR}/Packages/glm-1.0.1-light")
set (ASSIMP_DIR "${CMAKE_SOURCE_DIR}/Packages/assimp")


set(CMAKE_AUTOMOC ON)   # 自动运行 moc，处理 Q_OBJECT 宏，生成元对象代码（信号槽机制依赖）
set(CMAKE_AUTOUIC ON)   # 自动处理 .ui 文件，生成对应的界面代码
set(CMAKE_AUTORCC ON)   # 自动处理 .qrc 资源文件，生成资源访问代码

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "code/*.h"  
    "code/*.cpp"
    "control/*.h"
    "control/*.cpp"
    "control/*.png"
)

foreach(FILE_PATH IN LISTS SRC_FILES)
    file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${FILE_PATH}")
    get_filename_component(FILE_DIR "${REL_PATH}" DIRECTORY)
    source_group("${FILE_DIR}" FILES "${FILE_PATH}")
endforeach()

qt_add_resources(RESOURCES resources.qrc)




# 创建可执行文件，根据构建类型动态设置 WIN32 选项
#add_executable(QtGuiApp WIN32 ${SRC_FILES} ${RESOURCES} res/icon/apprc.rc)  # Release 模式：无控制台
add_executable(QtGuiApp ${SRC_FILES} ${RESOURCES}  res/icon/apprc.rc)

# 添加预编译头支持
target_precompile_headers(QtGuiApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/code/pch.h")

target_include_directories(QtGuiApp
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
        ${GLM_DIR}
        ${ASSIMP_DIR}/include
)
target_link_directories(QtGuiApp
    PRIVATE
        "${CMAKE_SOURCE_DIR}/lib"
        ${OpenCV_LIB_DIR}
        ${ASSIMP_DIR}/lib
)
target_link_libraries(QtGuiApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::SerialPort
    ${OpenCV_LIBS}
    assimp-vc143-mt.lib
    zlibstatic.lib
    opengl32.lib    
)


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

# 复制 assimp 和 zlib 的 DLL 到可执行文件目录
add_custom_command(TARGET QtGuiApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ASSIMP_DIR}/bin/assimp-vc143-mt.dll"
        "${ASSIMP_DIR}/bin/assimp-vc143-mtd.dll"
        $<TARGET_FILE_DIR:QtGuiApp>
)

