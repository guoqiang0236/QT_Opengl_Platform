#include "GLFWApplication.h"
#include <QDebug>

GLFWApplication::GLFWApplication(QObject* parent)
    : QObject(parent)
{
    m_width = 0;
    m_height = 0;
}

GLFWApplication* GLFWApplication::m_instance = nullptr;

GLFWApplication* GLFWApplication::Instance() {
    if (!m_instance) {
        m_instance = new GLFWApplication();
    }
    return m_instance;
}


GLFWApplication::~GLFWApplication()
{
	// 释放GLFW资源
	glfwTerminate();
    if(m_glfwwindow)
	{
		glfwDestroyWindow(m_glfwwindow);
		m_glfwwindow = nullptr;
	}
}

bool GLFWApplication::Initialize(const int& width, const int& height)
{
    m_width = width;
    m_height = height;
    if (binitialized)
        return false;

    // 调用 glfwInit() 初始化 GLFW 库，返回值为 GLFW_TRUE 表示成功
    if (glfwInit() == GLFW_TRUE) {
        binitialized = true;
        qDebug() << "GLFW 初始化成功";
        emit glfwInitialized(true);
    }
    else {
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

    binitialized = CreateOpenglWindow();

    //设置函数指针
    setResizeCallback(OnResize);
	setKeyBoardCallback(KeyBoardEvent);

    return binitialized;
}



bool GLFWApplication::CreateOpenglWindow()
{

    // 创建GLFW 窗口，并提供自定义删除器
    m_glfwwindow = glfwCreateWindow(m_width, m_height, "Hello GLFW", nullptr, nullptr);

    if (m_glfwwindow == NULL) {
        qWarning() << "GLFW 窗口创建失败";
        return false;
    }
	// 设置当前上下文为创建的窗口
	glfwMakeContextCurrent(m_glfwwindow);


    // 设置帧缓冲区大小回调函数
    glfwSetFramebufferSizeCallback(m_glfwwindow, FrameBufferSizeCallback);

    //设置键盘监听回调函数
	glfwSetKeyCallback(m_glfwwindow, KeyeCallback);

    //使用glad加载当前版本opengl函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        qDebug() << "glad初始化失败";
        return false;
    }
    qDebug() << "glad初始化成功";
    qDebug() << "GLFW 窗口创建成功";
    return true;
}

void GLFWApplication::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    if (GLFWApplication::Instance()->m_resizeCallback != nullptr)
    {
        GLFWApplication::Instance()->m_resizeCallback(width, height);
    }
}

void GLFWApplication::KeyeCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (GLFWApplication::Instance()->m_keyboardCallback != nullptr)
    {
        GLFWApplication::Instance()->m_keyboardCallback(key, scancode, action, mods);
    }
}

void GLFWApplication::OpenglWindowExec()
{
    if (!m_glfwwindow)
        return;
	while (!glfwWindowShouldClose(m_glfwwindow))
	{
		// 接受并分发窗体消息处理事件
		glfwPollEvents();

        //执行gl清画布操作
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        //执行gl双缓存操作
		glfwSwapBuffers(m_glfwwindow);
	}
}

void GLFWApplication::UseOpenGLFunctions()
{
	if (!m_glfwwindow) {
		qWarning() << "OpenGL 窗口未创建，无法使用 OpenGL 函数";
		return;
	}
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLenum err = glGetError();
  
	//qDebug() << "OpenGL 错误码: " << err;
}

void GLFWApplication::TerminateOpenglWindow()
{
    // 释放GLFW资源
    if (m_glfwwindow) {
        glfwDestroyWindow(m_glfwwindow);
        m_glfwwindow = nullptr;
		binitialized = false;
    }
}

uint32_t GLFWApplication::getWidth()
{
    return m_width;
}

uint32_t GLFWApplication::getHeight()
{
    return m_height;
}

void GLFWApplication::setResizeCallback(ResizeCallback callback)
{
    m_resizeCallback = callback;
}

void GLFWApplication::setKeyBoardCallback(KeyBoardCallback callback)
{
    m_keyboardCallback = callback;
}

void GLFWApplication::prepareVBO()
{
    /*VBO（Vertex Buffer Object，顶点缓冲对象）是OpenGL中的一种缓冲区对象，用于在显存（GPU内存）中高效存储顶点数据。
    作用
        •	高效传输数据：VBO允许你一次性把大量顶点数据（如位置、颜色、法线、纹理坐标等）传到GPU，避免每帧都从CPU传递数据，极大提升渲染效率。
        •	减少CPU与GPU通信：数据存储在GPU后，渲染时只需告诉OpenGL用哪个VBO即可，减少了CPU和GPU之间的数据交换。
        •	支持批量渲染：适合现代图形管线的批量处理，能一次性渲染大量顶点。*/
     
    //1、创建一个vbo
	GLuint vbo = 0;
    GL_CALL(glGenBuffers(1, &vbo));

	//2、销毁一个vbo
    GL_CALL(glDeleteBuffers(1, &vbo));

    //3、创建一堆vbo
    GLuint vboArr[] = {0,0,0};
    
    GL_CALL(glGenBuffers(3, vboArr));
    //4、销毁一堆vbo
    GL_CALL(glDeleteBuffers(3, vboArr));
}

void GLFWApplication::prepare()
{

    float vertices[] = {
        // 顶点坐标
        -0.5f, -0.5f, 0.0f,  // 左下角
         0.5f, -0.5f, 0.0f,  // 右下角
         0.0f,  0.5f, 0.0f   // 顶部
    };
    //1、创建一个vbo
    GLuint vbo = 0;
    GL_CALL(glGenBuffers(1, &vbo));
    //2.绑定当前vbo，到opengl状态机的vbo插槽上
    //GL_ARRAY_BUFFER表示当前vbo插槽
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

	//3,想当前vbo中传输数据，也是在开辟线程
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
}

void GLFWApplication::prepareSingleBuffer()
{
	//1.准备顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // 左下角
         0.5f, -0.5f, 0.0f,  // 右下角
         0.0f,  0.5f, 0.0f   // 顶部
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f, // 红色
        0.0f, 1.0f, 0.0f, 1.0f, // 绿色
        0.0f, 0.0f, 1.0f, 1.0f  // 蓝色
    };
    //2.使用数据生成两个vbo psovbo，colorvbo
    GLuint posvbo, colorvbo;
	GL_CALL(glGenBuffers(1, &posvbo));
    GL_CALL(glGenBuffers(1, &colorvbo));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posvbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorvbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));

    //3.生成vao并且绑定
    GLuint vao = 0;
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    //4.分别将位置/颜色属性的描述信息加入到vao中
    //4.1描述位置属性
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posvbo));
    GL_CALL(glEnableVertexAttribArray(0)); // 位置属性索引为0
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

	//4.2描述颜色属性
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorvbo));
    GL_CALL(glEnableVertexAttribArray(1)); // 颜色属性索引为1
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

    GL_CALL(glBindVertexArray(0)); //解绑VAO
}

void GLFWApplication::prepareInterleaveBuffer()
{
    //1.准备顶点数据(位置加颜色）
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
         0.0f,  0.5f, 0.0f ,  0.0f, 0.0f, 1.0f,
    };

    //2.创建唯一的vbo
    GLuint vbo = 0;
    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    //3.创建vao并绑定
	GLuint vao = 0;
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    //4.分别将位置/颜色属性的描述信息加入到vao中
    //4.1描述位置属性
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

    //4.2描述颜色属性
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glEnableVertexAttribArray(1)); // 颜色属性索引为1
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float))));

    GL_CALL(glBindVertexArray(0)); //解绑VAO

}



void OnResize(int width, int height)
{
    GL_CALL(glViewport(0, 0, width, height));
}

void KeyBoardEvent(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W) {
        qDebug() << "按下了: " << key << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods;
    }
}
