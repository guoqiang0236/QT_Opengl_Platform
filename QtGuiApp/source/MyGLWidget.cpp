#include "MyGLWidget.h"
#include <QDebug>

MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

MyGLWidget::~MyGLWidget()
{
    makeCurrent();
    if (m_program) {
		glDeleteProgram(m_program);
        m_program = 0;
    } 
    doneCurrent();
}
void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    prapareBackground();
   
    // 你可以在这里自动调用 prepareShader/prepareInterleaveBuffer 等
    //prepareShader();
    //prepareInterleaveBuffer();
}

void MyGLWidget::prapareBackground()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void MyGLWidget::prepareShader()
{
    //1 完成vs与fs的源代码，并且装入字符串
    const char* vertexShaderSource =
        "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    //2 创建Shader程序（vs、fs）
    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    //3 为shader程序输入shader代码
	glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
    glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);

    int success = 0;
    char infoLog[1024];
    //4 执行shader代码编译 
    glCompileShader(vertex);
    //检查vertex编译结果
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
		glGetShaderInfoLog(vertex, 1024, nullptr, infoLog);
		qDebug() << "Vertex Shader compilation failed:" << infoLog;
    }
    else
    {
        qDebug() << "Vertex Shader compilation successed";
    }

    //执行shader代码编译
    glCompileShader(fragment);
    //检查fragment编译结果
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
		glGetShaderInfoLog(fragment, 1024, nullptr, infoLog);
		qDebug() << "Fragment Shader compilation failed:" << infoLog;
    }
    else
    {
	    qDebug() << "Fragment Shader compilation successed";
    }

    //5 创建一个Program壳子
    m_program = glCreateProgram();

    //6 将vs与fs编译好的结果放到program这个壳子里
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);

    //7 执行program的链接操作，形成最终可执行shader程序
    glLinkProgram(m_program);

	//检查program链接结果
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
		glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
		qDebug() << "Shader Program linking failed:" << infoLog;

    }
    else
    {
        qDebug() << "Shader Program linking successed";
    }
    //清理
    glDeleteShader(vertex);
    glDeleteShader(fragment);


}

void MyGLWidget::prepareVBO()
{
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glDeleteBuffers(1, &vbo);

    GLuint vboArr[] = { 0, 0, 0 };
    glGenBuffers(3, vboArr);
    glDeleteBuffers(3, vboArr);
}

void MyGLWidget::prepareEBOVAO()
{
    //1 准备positions
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };
    //2 VBO创建
    m_vbo = 0;
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    //3 EBO创建
    m_ebo = 0;
    glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //4 VAO创建
	glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //5 绑定vbo ebo 加入属性描述信息
    //5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
    //5.2 加入索引属性描述信息
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);   

    glBindVertexArray(0);

}

void MyGLWidget::prepare()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void MyGLWidget::prepareSingleBuffer()
{
    //1 准备positions colors数据
    float positions[] = {
           -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };
    float colors[] = {
       1.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f,
       0.0f,  0.0f, 1.0f
    };


    //2 使用数据生成两个vbo posVbo， colorVbo
    GLuint posVbo, colorVbo;
    glGenBuffers(1, &posVbo);
    glGenBuffers(1, &colorVbo);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    //3 生成vao并且绑定
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //4 分别将位置/颜色属性的描述信息加入vao当中
    //4.1描述位置属性
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);//只有绑定了posVbo，下面的属性描述才会与此vbo相关
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //4.2 描述颜色属性
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}


void MyGLWidget::prepareInterleaveBuffer()
{
    //1 准备好Interleaved数据（位置+颜色）
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
    };

    //2 创建唯一的vbo
    m_vbo = 0;
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //3 创建并绑定vao
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

    
    //4 为vao加入位置和颜色的描述信息
    //4.1 位置描述信息
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);


	//4.2 颜色描述信息
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    //5 扫尾工作：解绑当前vao
    glBindVertexArray(0);

}

void MyGLWidget::render()
{
   //执行opengl画布清理操作
   glClear(GL_COLOR_BUFFER_BIT);
   //1 绑定当前的program
   glUseProgram(m_program);
   //2 绑定当前的vao
   glBindVertexArray(m_vao);
   //3 发出绘制指令
   glDrawArrays(GL_LINE_STRIP, 0, 6); // 绘制三角形
}

void MyGLWidget::prepareVAOForGLTriangles()
{
    //1 准备positions
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
        0.8f,  0.8f, 0.0f,
        0.8f,  0.0f, 0.0f
    };
    //2  posVbo
	GLuint posVbo = 0;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    //3 生成vao并且绑定
    glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

    //4 描述位置属性
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //5 扫尾工作：解绑当前vao
    glBindVertexArray(0);
}

void MyGLWidget::paintGL()
{
    render();
}

void MyGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W) {
        qDebug() << "按下了: W";
    }
    QOpenGLWidget::keyPressEvent(event); // 保留父类行为
}
void MyGLWidget::triggerDrawTriangle()
{
    // 你可以在这里按需调用各种prepare函数
    makeCurrent();
    prepareShader();
    //prepareInterleaveBuffer();
    prepareEBOVAO();
    m_prepared = true;
    doneCurrent();
    update(); // 触发重绘
}

