#include "GLFWHelper.h"
#include <QDebug>

GLFWHelper::GLFWHelper(QObject* parent)
    : QObject(parent)
{
}

bool GLFWHelper::initialize()
{
    if (initialized)
        return true;

    if (glfwInit() == GLFW_TRUE) {
        initialized = true;
		qDebug() << "GLFW 初始化成功";
        emit glfwInitialized(true);
    } else {
        initialized = false;
        emit glfwInitialized(false);
        qWarning() << "GLFW 初始化失败";
    }

    return initialized;
}