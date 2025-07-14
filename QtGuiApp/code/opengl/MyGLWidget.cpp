#include "MyGLWidget.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "Material/MyPhongMaterial.h"
#include "Material/MyWhiteMaterial.h"
MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    m_angle(0.0f),
    m_transform(glm::mat4(1.0f)),// 初始化变换矩阵为单位矩阵
    m_viewMatrix(glm::identity<glm::mat4>()),
	m_normalMatrix(glm::identity<glm::mat3>())

{
    
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    setMouseTracking(false); // 关键：开启鼠标跟踪

    m_timer.start();

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
   
   
}

void MyGLWidget::prapareBackground()
{
   
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
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

void MyGLWidget::preparecolortriangleShader()
{
    //1 完成vs与fs的源代码，并且装入字符串
    const char* vertexShaderSource =
        "#version 450 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   color = aColor;\n"
        "}\0";
    const char* fragmentShaderSource =
        "#version 450 core\n"
        "out vec4 FragColor;\n"
        "in vec3 color;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(color, 1.0f);\n"
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

    //检查链接错误
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

void MyGLWidget::prepareVAOcolortriangle()
{
    //1 准备positions colors
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        0.0f, 0.0f,1.0f
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    //2 VBO创建
    m_vbo = 0;
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    GLuint colorVbo = 0;
	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    //3 EBO创建
    m_ebo = 0;
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //4 VAO创建
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

    GLuint posLocation = 0;
	GLuint colorLocation = 0;
    //动态获取shaer的location
    if (m_Shader)
    {
        posLocation = glGetAttribLocation(m_Shader->getProgram(), "aPos");
        colorLocation = glGetAttribLocation(m_Shader->getProgram(), "aColor");
    }
    
	//5 绑定vbo ebo 加入属性描述信息
	//5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //5.2 加入颜色属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//5.3 加入索引属性描述数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	//6 扫尾工作：解绑当前vao
	glBindVertexArray(0);

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

void MyGLWidget::prepareStates()
{
	glEnable(GL_DEPTH_TEST); // 启用深度测试
	glDepthFunc(GL_LESS); // 设置深度测试函数

	glClearDepth(1.0f);// 设置深度缓冲的清除值
}

void MyGLWidget::prepareCamera()
{
    float size = 3.0f;

	m_camera = new MyPerspectiveCamera(
        60.0f, 
        (float)width() / (float)height(), 
        0.1f, 1000.0f,
        this);
    /*
    m_camera = new MyOrthographicCamera(
        -size, size,
        size, -size,
        size, -size,
        this
    );*/
	m_cameraControl = new MyTrackBallCameraControl(this);
	//m_cameraControl = new MyGameCameraControl(this);
    m_cameraControl->setcamera(m_camera);
	
    
}

void MyGLWidget::prepareOrtho()
{
	//ortho:生成一个正交投影矩阵
	//left, right, bottom, top, near, far
	//注意：OpenGL的正交投影矩阵是左手坐标系
	//注意：OpenGL的正交投影矩阵是左下角为原点
	//注意：OpenGL的正交投影矩阵是z轴朝外的
	m_projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f);
}

void MyGLWidget::preparareProjection()
{
	m_projectionMatrix = glm::perspective(
		glm::radians(80.0f), // 视野角度
		(float)width() / (float)height(), // 宽高比
		0.1f, // 近裁剪面
		100.0f // 远裁剪面
	);
}

void MyGLWidget::render()
{
   //执行opengl画布清理操作
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 清除颜色缓冲和深度缓冲
   //1 绑定当前的program
   if (m_Shader)
   {
       m_Shader->begin();

       m_Shader->setInt("sampler", 0);// 作用是把 shader 里的 sampler uniform 变量和 GL_TEXTURE0 纹理单元关联起来
       //设定全局uniform time
       m_Shader->setFloat("time", 1.0);
	   //m_Shader->setFloat("time", m_timer.elapsed() / 1000.0f); // 传递时间给着色器

	   m_Shader->setMatrix4x4("modelMatrix", m_transform); // 传递单位矩阵作为变换矩阵
	   m_normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_transform))); // 计算法线矩阵))
	   m_Shader->setMatrix3x3("normalMatrix", m_normalMatrix); // 传递法线矩阵

       //摄像机
       if (m_camera)
       {
           m_Shader->setMatrix4x4("viewMatrix", m_camera->getViewMatrix());// 传递viewMatrix摄像机矩阵
           m_Shader->setMatrix4x4("projectionMatrix", m_camera->getProjectionMatrix());
           m_Shader->setVector3("cameraPosition", m_camera->mPosition);
	   }
	 
       //光源参数
       m_Shader->setVector3("lightDirection", m_lightDirection);
       m_Shader->setVector3("lightColor", m_lightColor);
	   m_Shader->setFloat("specularIntensity", m_specularIntensity); // 镜面反射强度
       m_Shader->setVector3("ambientColor", m_ambientColor);
   }
   
 
   //2 绑定当前的vao
   if (m_geometry)
   {
       glBindVertexArray(m_geometry->getVao());
   }
   //3 发出绘制指令
   
   //第一次绘制
   if (m_Texture) {
       m_Texture->bind();
   }
   if (m_geometry)
   {
       glDrawElements(GL_TRIANGLES, m_geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0)); // 使用索引绘制

   }

   //第二次绘制

   //if (m_Texture2)
   //{
   //    m_Texture2->bind();
   //    m_transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.8f, 0.0f, -1.0f));
   //    m_Shader->setMatrix4x4("transform", m_transform2);
   //}
   //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(int) * 0)); // 使用索引绘制
   
   glBindVertexArray(0);

   if (m_Shader)
       m_Shader->end();
}

void MyGLWidget::doRotationTransform()
{	//构建一个旋转矩阵，绕着z轴旋转45度角
    //rotate函数：用于生成旋转矩阵
    //bug1:rotate必须得到一个float类型的角度，c++的template
    //bug2:rotate函数接受的不是角度（degree），接收的弧度（radians）
    //注意点：radians函数也是模板函数，切记要传入float类型数据，加f后缀
    // 
    m_transform = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}


void MyGLWidget::prepare() {
    m_renderer = new::MyRenderer();

    // 1. 创建geometry
    auto geometry = MyGeometry::createBox(1.0f);

    // 2. 创建一个material并且配置参数
    auto material01 = new MyPhongMaterial();
    material01->mShiness = 32.0f;
    material01->mDiffuse = new MyTexture("../assets/textures/box.png", 0); // 兼容原有指针成员
    material01->mSpecularMask = new MyTexture("../assets/textures/sp_mask.png", 1);

    // 3. 生成mesh并用智能指针管理
    auto mesh01 = new::MyMesh(geometry, material01);


	auto geometryWhite = MyGeometry::createSphere(0.05f);
    auto materialWhite = new MyWhiteMaterial();
    auto meshWhite = new MyMesh(geometryWhite, materialWhite);
    meshWhite->setPosition(glm::vec3(2.1, 0.0, 0.0));
    

    // material 也可以存到材质列表里，或由 mesh 持有
    m_meshes.push_back(mesh01);
    m_meshes.push_back(meshWhite);

    //m_dirLight = new::MyDirectionalLight(); // 平行光
    // 创建点光源并设置衰减参数
    m_pointLight = new MyPointLight();
    m_pointLight->setPosition(meshWhite->getPosition());
    m_pointLight->mK2 = 0.017f;  // 二次衰减系数
    m_pointLight->mK1 = 0.07f;   // 线性衰减系数
    m_pointLight->mKc = 1.0f;    // 常数衰减系数
    m_ambLight = new::MyAmbientLight(); // 环境光
	m_ambLight->mColor = glm::vec3(0.2f, 0.2f, 0.2f); // 设置环境光颜色
}

void MyGLWidget::doTranslationTransform()
{
    //平移变换
	m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
}

void MyGLWidget::doScaleTransform()
{
    //缩放变换
    m_transform = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
}

void MyGLWidget::doTransform()
{
    //复合变换
    //glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));

    //先旋转 再平移
    //m_transform = translateMat * rotateMat;

    //先平移 后旋转
    //m_transform = rotateMat * translateMat;

    m_transform = glm::rotate(m_transform, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 1.0f));
}

void MyGLWidget::doRotation()
{
	//持续旋转
    m_angle += 1.0f;
    m_transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_angle), glm::vec3(0.0f, 0.0f, 1.0f));
}

void MyGLWidget::preTransformDieJia()
{
    //目标二：先平移再叠加旋转
    //m_transform = glm::translate(m_transform, glm::vec3(0.6f, 0.0f, 0.0f));

    //目标三：先旋转再叠加平移
    //m_transform = glm::rotate(m_transform, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    //目标四：先做一次缩放，再叠加平移 
    m_transform = glm::scale(m_transform, glm::vec3(2.0f, 1.0f, 1.0f));
}

void MyGLWidget::doTransformDieJia()
{
    //目标一：旋转的三角形
    //m_transform = glm::rotate(m_transform, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    //目标二：先平移再叠加旋转
    //m_transform = glm::rotate(m_transform, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    //目标三：先旋转再叠加平移
    //m_transform = glm::translate(m_transform, glm::vec3(0.01f, 0.0f, 0.0f));

    //目标四：先做一次缩放，再叠加平移 
	m_transform = glm::translate(m_transform, glm::vec3(0.01f, 0.0f, 0.0f));
}



void MyGLWidget::prepareShaderPtr()
{
    //qDebug() << QDir::currentPath();
    m_Shader = std::make_unique<MyShader>("../assets/shaders/vertex.glsl", "../assets/shaders/fragment.glsl");
}

void MyGLWidget::prepareShaderPtrForMat()
{
	m_Shader = std::make_unique<MyShader>("../assets/shaders/vertexMat.glsl", "../assets/shaders/fragmentMat.glsl");
}

void MyGLWidget::prepareShaderPtrForGeometry()
{
	m_Shader = std::make_unique<MyShader>("../assets/shaders/vertexGeometry.glsl", "../assets/shaders/fragmentGeometry.glsl");
}

void MyGLWidget::prepareTexture()
{
    // 1. 用 OpenCV 读取图片（注意：imread 默认是 BGR）
    cv::Mat img = cv::imread("../assets/textures/hinata.jpg", cv::IMREAD_UNCHANGED);
    if (img.empty()) {
        qDebug() << "图片加载失败";
        return;
    }
    //OpenCV 默认左上角为原点，OpenGL 纹理左下角为原点，需要上下翻转
    cv::flip(img, img, 0);
    //转为 RGBA 格式（如果不是4通道）
    if (img.channels() == 3) {
        cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);
    }
    else if (img.channels() == 4) {
        cv::cvtColor(img, img, cv::COLOR_BGRA2RGBA);
    }

    //2 生成纹理并且激活单元绑定
    glGenTextures(1, &m_texture);
    //--激活纹理单元--
    glActiveTexture(GL_TEXTURE0);
    //--绑定纹理对象--
    glBindTexture(GL_TEXTURE_2D, m_texture);

    //3 传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows,
        0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);

    //4 设置纹理的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //5 设置纹理的包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v

}

void MyGLWidget::prepareVAOForGLTrianglesWithTexture()
{
    //1 准备positions colors
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        0.0f, 0.0f,1.0f
    };

    float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2
    };
	//2 VBO创建
    GLuint posVbo, colorVbo, uvVbo;
    glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO创建
	m_ebo = 0;
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //4 VAO创建
	glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLuint posLocation = 0;
    GLuint colorLocation = 0;
    GLuint uvlocation = 0;
    //动态获取shaer的location
    if (m_Shader)
    {
        posLocation = glGetAttribLocation(m_Shader->getProgram(), "aPos");
        colorLocation = glGetAttribLocation(m_Shader->getProgram(), "aColor");
		uvlocation = glGetAttribLocation(m_Shader->getProgram(), "aUV");
    }

    //5 绑定vbo ebo 加入属性描述信息
    //5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(posLocation);
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float),(void*)0);

	//5.2 加入颜色属性描述数据
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0);

    //5.3 加入uv属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(uvlocation);
    glVertexAttribPointer(uvlocation,2,GL_FLOAT,GL_FALSE,
		2 * sizeof(float), (void*)0);

	//5.4 加入索引属性描述数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBindVertexArray(0);
}

void MyGLWidget::prepareVAOForTexture()
{
    //1 准备positions colors
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        0.0f, 0.0f,1.0f,
        0.5f, 0.5f,0.5f
    };

    float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    //2 VBO创建
    GLuint posVbo, colorVbo, uvVbo;
    glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO创建
    GLuint ebo;
	glCreateBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //4 VAO创建
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //5 绑定vbo ebo 加入属性描述信息
    GLuint posLocation = 0;
    GLuint colorLocation = 0;
    GLuint uvlocation = 0;
    //动态获取shaer的location
    if (m_Shader)
    {
        posLocation = glGetAttribLocation (m_Shader->getProgram(), "aPos");
        colorLocation = glGetAttribLocation(m_Shader->getProgram(), "aColor");
        uvlocation = glGetAttribLocation(m_Shader->getProgram(), "aUV");
    }
    //5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(posLocation);
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    //5.2 加入颜色属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    //5.3 加入uv属性描述数据
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(uvlocation);
	glVertexAttribPointer(uvlocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);


    //5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindVertexArray(0);

}

void MyGLWidget::prepareVAOForMipmapTexture()
{
    //1 准备positions colors
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        0.0f, 0.0f,1.0f,
    };

    float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
    };


	//2 VBO创建
    GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    //3 EBO创建
    GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //5 绑定vbo ebo 加入属性描述信息
    //5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //5.2 加入颜色属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //5.3 加入uv属性描述数据
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,2 * sizeof(float), (void*)0);

    //5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//6 扫尾工作：解绑当前vao
    glBindVertexArray(0);
}

void MyGLWidget::prepareVAOForLiuYiFei()
{
    //1 准备positions colors
    float positions[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        0.0f, 0.0f,1.0f,
        0.5f, 0.5f,0.5f
    };

    //•	UV坐标（也叫纹理坐标）确实是用来告诉采样器（sampler）在图片（纹理）上“采集”哪一部分内容的
    float uvs[] = {
        0.0f, 0.0f,//左下角
        1.0f, 0.0f,//右下角
        0.0f, 1.0f,//左上角
        1.0f, 1.0f,//右上角
    };
        
    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };
    //2 VBO创建
    GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    //3 EBO创建
    GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //4 VAO创建
    glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

    //5 绑定vbo ebo 加入属性描述信息
    GLuint posLocation = 0;
    GLuint colorLocation = 0;
    GLuint uvlocation = 0;
    //动态获取shaer的location
    if (m_Shader)
    {
        posLocation = glGetAttribLocation(m_Shader->getProgram(), "aPos");
        colorLocation = glGetAttribLocation(m_Shader->getProgram(), "aColor");
        uvlocation = glGetAttribLocation(m_Shader->getProgram(), "aUV");
    }

    //5.1 加入位置属性描述信息
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(posLocation);
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //5.2 加入颜色属性描述数据
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //5.3 加入uv属性描述数据
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glEnableVertexAttribArray(uvlocation);
    glVertexAttribPointer(uvlocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //5.4 加入ebo到当前的vao
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    //6 扫尾工作：解绑当前vao
    glBindVertexArray(0);

}

void MyGLWidget::prepareVAOForGeometry()
{
    //m_geometry = MyGeometry::createBox(6.0f);
    m_geometry = MyGeometry::createSphere(1.0f);
	//m_geometry = MyGeometry::createPlane(1.0f, 1.0f);
}

void MyGLWidget::prepareTexturePtr()
{
	m_Texture = std::make_unique<MyTexture>("../assets/textures/hinata.jpg", 0);
}

void MyGLWidget::prepareMipmapTexturePtr()
{
    //m_Texture2 = std::make_unique<MyTexture>("../assets/textures/hinata.jpg", 0);
    m_Texture = std::make_unique<MyTexture>("../assets/textures/earth.jpg", 0);
	
}

void MyGLWidget::prepareMixTexturePtr()
{
    grassTexture = new MyTexture("../assets/textures/grass.jpg", 0);
    landTexture = new MyTexture("../assets/textures/land.jpg", 1);
    noiseTexture = new MyTexture("../assets/textures/noise.jpg", 2);
}

void MyGLWidget::prepareMipmapLiuYiFeiTexturePtr()
{
	m_Texture = std::make_unique<MyTexture>("../assets/textures/liu.jpg", 1);//1是纹理单元GL_TEXTURE 1
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
    //摄像机更新
    if (m_cameraControl)
    {
        m_cameraControl->update();
    }
    if (m_renderer && !m_meshes.empty())
    {
        //m_meshes[0]->rotateY(0.1f);
        m_animTime += 0.02f; // 控制速度
        float amplitude = 1.0f; // 控制最大平移距离
        float x = std::sin(m_animTime) * amplitude;

		glm::vec3 pos = m_meshes[0]->getPosition();
		m_meshes[0]->setPosition(glm::vec3(x, pos.y, pos.z)); // 更新位置
        m_renderer->render(m_meshes,m_camera, m_pointLight,m_ambLight);
    }
    //变换矩阵
    //doTransform();
    //doTransformDieJia();
    //渲染
    //render();
    update();
}

void MyGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

MyShader* MyGLWidget::getShader() const
{
    return m_Shader.get();
}

GLuint MyGLWidget::getProgram() const
{
    return m_program;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{

    if(m_cameraControl)
	{
       // 1 表示按下，0 表示释放（你可以自定义，常用1=Press, 0=Release）
		m_cameraControl->onKey(event->key(),1,event->modifiers());
	}
    QOpenGLWidget::keyPressEvent(event); // 保留父类行为
}
void MyGLWidget::keyReleaseEvent(QKeyEvent* event)
{

    if (m_cameraControl)
    {
        // 1 表示按下，0 表示释放（你可以自定义，常用1=Press, 0=Release）
        m_cameraControl->onKey(event->key(), 0, event->modifiers());
    }
    QOpenGLWidget::keyPressEvent(event); // 保留父类行为
}
void MyGLWidget::mousePressEvent(QMouseEvent* event)
{
    //qDebug() << "鼠标点击: 按钮" << event->button() << ", 按下";
    if (m_cameraControl)
    {
        // 1 表示按下，0 表示释放（你可以自定义，常用1=Press, 0=Release）
        m_cameraControl->onMouse(static_cast<int>(event->button()), 1, event->position().x(),event->position().y());
    }
    // 其他处理逻辑...
    QOpenGLWidget::mousePressEvent(event); // 保留父类行为（可选）
}
void MyGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    //qDebug() << "鼠标点击: 按钮" << event->button() << ", 松开";
    // 其他处理逻辑...
    if (m_cameraControl)
    {
        // 1 表示按下，0 表示释放（你可以自定义，常用1=Press, 0=Release）
        m_cameraControl->onMouse(static_cast<int>(event->button()), 0, event->position().x(), event->position().y());
    }
    QOpenGLWidget::mouseReleaseEvent(event); // 保留父类行为（可选）
}
void MyGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	//qDebug() << "鼠标移动: 位置" << event->pos();
	// 其他处理逻辑...
    if (m_cameraControl)
    {
        // 1 表示按下，0 表示释放（你可以自定义，常用1=Press, 0=Release）
        m_cameraControl->onCursor(event->position().x(), event->position().y());
    }
	QOpenGLWidget::mouseMoveEvent(event); // 保留父类行为（可选）
}
void MyGLWidget::wheelEvent(QWheelEvent* event)
{
	//qDebug() << "鼠标滚轮: 角度" << event->angleDelta();
	// 其他处理逻辑...
	if (m_cameraControl)
	{
        // 传递滚轮滚动的距离，正值为向上，负值为向下
		m_cameraControl->onScroll(event->angleDelta().y());
	}
	QOpenGLWidget::wheelEvent(event); // 保留父类行为（可选）
}
void MyGLWidget::showEvent(QShowEvent* event)
{
    QOpenGLWidget::showEvent(event);
    setFocus(Qt::OtherFocusReason); // 这里再次请求焦点
}
void MyGLWidget::triggerDrawTriangle()
{
    makeCurrent();
    prepareShaderPtr();
    prepareVAOForGLTrianglesWithTexture();
    prepareTexture();
    m_prepared = true;
    doneCurrent();
    update(); // 触发重绘
}

void MyGLWidget::triggerDrawTexture()
{
    makeCurrent();
    prepareShaderPtr();
    prepareVAOForTexture();
    prepareTexturePtr();
    m_prepared = true;
    doneCurrent();
	update(); // 触发重绘
}

void MyGLWidget::triggerDrawMixTexture()
{
    makeCurrent();
	prepareShaderPtr();
    prepareVAOForTexture();
    prepareMixTexturePtr();
    m_prepared = true;
    doneCurrent();
    update();
}

void MyGLWidget::triggerDrawMipmapTexture()
{
    makeCurrent();
    prepareShaderPtr();
    prepareVAOForTexture();
    //prepareVAOForMipmapTexture();
    prepareMipmapTexturePtr();
    m_prepared = true;
    doneCurrent();
    update();
}

void MyGLWidget::triggerDrawLiuYiFei()
{
    makeCurrent();
    prepareShaderPtrForMat();
    prepareVAOForLiuYiFei();
    prepareMipmapLiuYiFeiTexturePtr();
    preparareProjection();
    prepareCamera();
  
    m_prepared = true;
    doneCurrent();
    update();
}

void MyGLWidget::triggerDrawTwoPictures()
{
    makeCurrent();
    prepareShaderPtrForMat();
    prepareVAOForLiuYiFei();
    prepareMipmapTexturePtr();
    prepareCamera();
    prepareStates();
    m_prepared = true;
    doneCurrent();
    update();
}

void MyGLWidget::triggerDrawGeometryBox()
{
    makeCurrent();
    prepareShaderPtrForGeometry();
    prepareVAOForGeometry();
    prepareMipmapTexturePtr();
    prepareCamera();
	prepareStates();
    m_prepared = true;
    doneCurrent();
    update();
}

void MyGLWidget::triggerDraw()
{
    makeCurrent();
    prepareCamera();
    prepare();
    m_prepared = true;
    doneCurrent();
    update();
}
