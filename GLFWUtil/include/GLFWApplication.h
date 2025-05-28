#pragma once
#include <glad/glad.h>
#include <QObject>
#include "pch.h"
#include "GLFWUtilExport.h"
#include "GLFW/glfw3.h"
#include "CheckError.h"


void OnResize(int width, int height);
void KeyBoardEvent(int key, int scancode, int action, int mods);
using ResizeCallback = void(*)(int width, int height);
using KeyBoardCallback = void(*)(int key, int scancode, int action, int mods);

class GLFWUTIL_API GLFWApplication : public QObject
{
    Q_OBJECT

public:
    // 获取单例实例
    static GLFWApplication* Instance();

    // 删除拷贝构造和赋值
    GLFWApplication(const GLFWApplication&) = delete;
    GLFWApplication& operator=(const GLFWApplication&) = delete;

    ~GLFWApplication();
    bool Initialize(const int& width = 800, const int& height = 600);
   
    void OpenglWindowExec();
    void UseOpenGLFunctions();
    void TerminateOpenglWindow();
    uint32_t getWidth();
    uint32_t getHeight();

    void setResizeCallback(ResizeCallback callback);
    void setKeyBoardCallback(KeyBoardCallback callback);

	void prepareVBO();

    void prepare();
    
    void prepareSingleBuffer();

    void prepareInterleaveBuffer();

signals:
    void glfwInitialized(bool success);

private:
    explicit GLFWApplication(QObject* parent = nullptr);
    static GLFWApplication* m_instance;
    bool CreateOpenglWindow();

    bool binitialized = false;
    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void KeyeCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	// OpenGL 窗口指针
    uint32_t m_width;
	uint32_t m_height;
    GLFWwindow* m_glfwwindow;
	ResizeCallback m_resizeCallback = nullptr;
    KeyBoardCallback m_keyboardCallback = nullptr;

};
