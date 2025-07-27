#include "MyGLWidget.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "Material/MyPhongMaterial.h"
#include "Material/MyWhiteMaterial.h"
#include "Material/MyImageMaterial.h"

namespace MyOpenGL {
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

    void MyGLWidget::setBackgroundColor(const QColor& color)
    {
        m_bgColor = color;
        update(); // 触发重绘
    }

    void MyGLWidget::prapareBackground(QColor color)
    {
        glClearColor(color.redF(), color.greenF(), color.blueF(), 1.0f);
    }

    void MyGLWidget::initializeGL()
    {

        initializeOpenGLFunctions();
        prapareBackground();


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

        m_camera = new OpenGLCamera::MyPerspectiveCamera(
            60.0f,
            (float)width() / (float)height(),
            0.1f, 1000.0f,
            this);

        /*  m_camera = new MyOrthographicCamera(
              -size, size,
              size, -size,
              size, -size,
              this
          );*/
        m_cameraControl = new OpenGLCamera::MyTrackBallCameraControl(this);
        //m_cameraControl = new MyGameCameraControl(this);
        m_cameraControl->setcamera(m_camera);


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

    void MyGLWidget::bShowLogo(bool bshow)
    {
        bhaslogo = bshow;
        update();
    }

    void MyGLWidget::bShowRenderer(bool bshow)
    {
        bhasrenderer = bshow;
        update();
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
        m_renderer = new::MyOpenGL::MyRenderer();
        m_scene = new ::MyOpenGL::MyScene();
        // 1. 创建geometry
        auto geometry = MyGeometry::createBox(3.0f);
        auto spgeometry = MyGeometry::createBox(1.0f);

        // 2. 创建一个material并且配置参数
        auto material01 = new MyPhongMaterial();
        material01->mShiness = 16.0f;
        material01->mDiffuse = new MyTexture("../assets/textures/box.png", 0); // 兼容原有指针成员
        material01->mSpecularMask = new MyTexture("../assets/textures/sp_mask.png", 1);

        // 3. 生成mesh并用智能指针管理
        auto mesh = new::MyOpenGL::MyMesh(geometry, material01);
        auto spmesh = new ::MyOpenGL::MyMesh(spgeometry, material01);
        spmesh->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));

        //创建父子关系
        mesh->addChild(spmesh);
        m_scene->addChild(mesh);

        auto geometryWhite = MyGeometry::createSphere(0.1f);
        auto materialWhite = new MyWhiteMaterial();
        auto meshWhite = new MyMesh(geometryWhite, materialWhite);
        meshWhite->setPosition(glm::vec3(0.8, 0.0, 0.0));


        // material 也可以存到材质列表里，或由 mesh 持有



        // 创建点光源并设置衰减参数 
        auto pointLight1 = new MyPointLight();
        pointLight1->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
        pointLight1->mColor = glm::vec3(1.0f, 0.0f, 0.0f); // 设置点光源颜色
        pointLight1->mK2 = 0.0f;        // 二次衰减系数
        pointLight1->mK1 = 0.0f;        // 线性衰减系数
        pointLight1->mKc = 1.0f;        // 常数衰减系数
        m_pointLights.push_back(pointLight1);

        auto pointLight2 = new MyPointLight();
        pointLight2->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
        pointLight2->mColor = glm::vec3(0.0f, 1.0f, 0.0f);
        pointLight2->mK2 = 0.0f;
        pointLight2->mK1 = 0.0f;
        pointLight2->mKc = 1.0f;
        m_pointLights.push_back(pointLight2);

        auto pointLight3 = new MyPointLight();
        pointLight3->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
        pointLight3->mColor = glm::vec3(0.0f, 0.0f, 1.0f);
        pointLight3->mK2 = 0.0f;
        pointLight3->mK1 = 0.0f;
        pointLight3->mKc = 1.0f;
        m_pointLights.push_back(pointLight3);

        auto pointLight4 = new MyPointLight();
        pointLight4->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
        pointLight4->mColor = glm::vec3(1.0f, 1.0f, 0.0f);
        pointLight4->mK2 = 0.0f;
        pointLight4->mK1 = 0.0f;
        pointLight4->mKc = 1.0f;
        m_pointLights.push_back(pointLight4);


        // 聚光灯
        m_spotLight = new ::MyOpenGL::MySpotLight();
        m_spotLight->setPosition(meshWhite->getPosition());
        m_spotLight->mTargetDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
        m_spotLight->mInnerAngle = 30.0f;
        m_spotLight->mOuterAngle = 45.0f;

        // 平行光
        m_dirLight = new::MyOpenGL::MyDirectionalLight();
        m_dirLight->mDirection = glm::vec3(1.0f, 1.0f, 1.0f);

        m_ambLight = new::MyOpenGL::MyAmbientLight(); // 环境光
        m_ambLight->mColor = glm::vec3(0.2f, 0.2f, 0.2f); // 设置环境光颜色

        bhasrenderer = true;
    }

    void MyGLWidget::preparelogo()
    {
        if (!m_renderer)
        {
            m_renderer = new::MyOpenGL::MyRenderer();
        }
        // 1. 创建geometry
        auto logogeometry = MyGeometry::createLogoQuad(100, 100);

        // 2. 创建一个material并且配置参数
        auto material01 = new MyImageMaterial();
        //material01->mShiness = 32.0f;
        //material01->mDiffuse = new MyTexture("../assets/textures/logo.left.top.bmp", 2); // 兼容原有指针成员


        // 3. 生成mesh并用指针管理
        //auto logo = new::MyMesh(logogeometry, material01);


        // material 也可以存到材质列表里，或由 mesh 持有
        //m_meshes.push_back(logo);

        logoVAO = logogeometry->getVao();
        m_Shader = std::make_unique<MyShader>("../assets/shaders/logo.vert", "../assets/shaders/logo.frag");
        logoTexture = new MyTexture("../assets/textures/logo_left_top.png", 2); // 兼容原有指针成员
        bhaslogo = true;
    }

    void MyGLWidget::preparemoxing()
    {
		m_renderer = new::MyOpenGL::MyRenderer();
        m_scene = new ::MyOpenGL::MyScene();
        
		auto testmodel = MyAssimpLoader::load("../assets/fbx/Fist Fight B.fbx");
        testmodel->setScale(glm::vec3(0.1f));
        if (!testmodel)
            return;
        m_scene->addChild(testmodel);


        //深度测试
		//auto geometry = MyGeometry::createPlane(5.0f,5.0f);
		//auto materialA = new MyPhongMaterial();
		//materialA->mDiffuse = new MyTexture("../assets/textures/goku.jpg", 0); // 兼容原有指针成员
		//auto meshA = new::MyOpenGL::MyMesh(geometry, materialA);
  //      m_scene->addChild(meshA);

  //      auto materialB = new MyPhongMaterial();
  //      materialB->mDiffuse = new MyTexture("../assets/textures/box.png", 0); // 兼容原有指针成员
  //      //materialB->mDepthWrite = false;
  //      auto meshB = new::MyOpenGL::MyMesh(geometry, materialB);
		//meshB->setPosition(glm::vec3(2.0f, 0.5f, -1.0f));
  //      m_scene->addChild(meshB);

  //      auto materialC = new MyPhongMaterial();
  //      materialC->mDiffuse = new MyTexture("../assets/textures/earth.png", 0); // 兼容原有指针成员
  //      auto meshC = new::MyOpenGL::MyMesh(geometry, materialC);
  //      meshC->setPosition(glm::vec3(4.0f, 1.0f, -2.0f));
  //      m_scene->addChild(meshC);

        //平行光
		m_dirLight = new::MyOpenGL::MyDirectionalLight();
        m_dirLight->mDirection = glm::vec3(-1.0f);
        m_dirLight->mSpecularIntensity = 0.4f;
        
        // 环境光
		m_ambLight = new ::MyOpenGL::MyAmbientLight(); 
		m_ambLight->mColor = glm::vec3(0.1f); 

        bhasrenderer = true;
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




    void MyGLWidget::paintGL()
    {
        //背景色
        prapareBackground(m_bgColor);
        //摄像机更新
        if (m_cameraControl)
        {
            m_cameraControl->update();
        }
        if (m_renderer && m_scene)
        {
            //m_meshes[0]->rotateY(0.1f);
            m_animTime += 0.02f; // 控制速度
            float amplitude = 1.0f; // 控制最大平移距离
            float x = std::sin(m_animTime) * amplitude;

            m_renderer->render(m_scene, m_camera, m_dirLight, m_pointLights, m_spotLight, m_ambLight, bhasrenderer);
        }
        if (bhaslogo && logoTexture != 0)
        {
            renderLogoOnScreen();
        }
  
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

        if (m_cameraControl)
        {
            // 1 表示按下，0 表示释放（你可以自定义，常用1=Press, 0=Release）
            m_cameraControl->onKey(event->key(), 1, event->modifiers());
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
            m_cameraControl->onMouse(static_cast<int>(event->button()), 1, event->position().x(), event->position().y());
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

    void MyGLWidget::triggerDraw()
    {
        makeCurrent();
        //prepare();
        preparemoxing();
        preparelogo();
        prepareCamera();

        m_prepared = true;
        emit prepareok(true);
        doneCurrent();
        update();
    }
    void MyGLWidget::renderLogoOnScreen()
    {
        // 假设你有 logoVAO, logoShader, logoTexture
        glBindVertexArray(logoVAO);
        m_Shader->begin();

        // 1. 构造正交投影矩阵（屏幕空间，左上角为原点）
        int w = width();
        int h = height();
        glm::mat4 ortho = glm::ortho(0.0f, float(w), float(h), 0.0f, -1.0f, 1.0f);

        // 2. 计算 logo 的模型矩阵（左上角，宽高为 logo 像素）
        float logoWidth = 10.0f;
        float logoHeight = 10.0f;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(logoWidth / 2, logoHeight / 2, 0.0f));
        glm::mat4 mvp = ortho * model;

        m_Shader->setMatrix4x4("mvp", mvp);
        m_Shader->setInt("logoTex", 2); // 0 表示 GL_TEXTURE0
        logoTexture->bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        m_Shader->end();
        glBindVertexArray(0);

    }

}