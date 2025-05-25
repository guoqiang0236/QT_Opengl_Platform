#include "GLFWHelper.h"
#include <QDebug>

GLFWHelper::GLFWHelper(QObject* parent)
    : QObject(parent)
{
}

GLFWHelper::~GLFWHelper()
{
	// 释放GLFW资源
	glfwTerminate();
    if(m_glfwwindow)
	{
		glfwDestroyWindow(m_glfwwindow);
		m_glfwwindow = nullptr;
	}
}

bool GLFWHelper::Initialize()
{
    if (binitialized)
        return true;

    // 调用 glfwInit() 初始化 GLFW 库，返回值为 GLFW_TRUE 表示成功
    if (glfwInit() == GLFW_TRUE) {
        binitialized = true;
		qDebug() << "GLFW 初始化成功";
        emit glfwInitialized(true);
    } else {
        binitialized = false;
        emit glfwInitialized(false);
        qWarning() << "GLFW 初始化失败";
    }
    // 设置 OpenGL 上下文主版本号为 4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // 设置 OpenGL 上下文次版本号为 6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // 设置 OpenGL 配置为核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    return binitialized;
}

bool GLFWHelper::CreateOpenglWindow()
{

    // 创建GLFW 窗口，并提供自定义删除器
    m_glfwwindow = glfwCreateWindow(800, 600, "Hello GLFW", nullptr, nullptr);

    if (!m_glfwwindow) {
        qWarning() << "GLFW 窗口创建失败";
        return false;
    }
	// 设置当前上下文为创建的窗口
	glfwMakeContextCurrent(m_glfwwindow);

    qDebug() << "GLFW 窗口创建成功";
    return true;
}

void GLFWHelper::OpenglWindowExec()
{
    if (!m_glfwwindow)
        return;
	while (!glfwWindowShouldClose(m_glfwwindow))
	{
		// 接受并分发窗体消息处理事件
		glfwPollEvents();
	}
}

void GLFWHelper::TerminateOpenglWindow()
{
    // 释放GLFW资源
    if (m_glfwwindow) {
        glfwDestroyWindow(m_glfwwindow);
        m_glfwwindow = nullptr;
    }
}

