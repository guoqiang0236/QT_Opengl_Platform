#include <QApplication>
#include <QQmlApplicationEngine>
#include <QWindow>
#include <QTimer>
#include "MainWindow.h"
#include "CustomSplashScreen.h"
#include <QFile>
#include "GlobalConfig.h"



int main(int argc, char* argv[]) {
    

    // ... 保持全局配置代码 ...
    sysconfig::GlobalConfig& config = sysconfig::GlobalConfig::get();
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 启用高DPI缩放
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);

    // 设置OpenGL版本和配置
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);

    //loading加载界面
    //CustomSplashScreen loadingflash;
    //loadingflash.init();
    // 加载样式表
    QFile qss(":/res/QSS/MacOS.qss");
    if (qss.open(QFile::ReadOnly)) {
        app.setStyleSheet(qss.readAll());
        qss.close();
    }
    auto mainWindow = std::make_unique<MainWindow>();
  
    
    mainWindow->show();

    //loadingflash.finish(mainWindow.get());
    return app.exec();
}