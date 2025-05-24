#pragma once

#include <QObject>
#include "pch.h"
#include "GLFWUtilExport.h"
#include "GLFW\glfw3.h"
class GLFWUTIL_API  GLFWHelper : public QObject
{
    Q_OBJECT

public:
    explicit GLFWHelper(QObject* parent = nullptr);

    // 初始化 GLFW，返回是否成功
    bool initialize();

signals:
    void glfwInitialized(bool success);

private:
    bool initialized = false;
};