#include "MyGLWidget.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "Material/MyPhongMaterial.h"
#include "Material/MyWhiteMaterial.h"
#include "Material/MyImageMaterial.h"
#include "Material/MydepthMaterial.h"
#include "Material/MyopacityMaskMaterial.h"
#include "Material/MyscreenMaterial.h"
#include "Material/MycubeMaterial.h"
#include "Material/MyphongEnvMaterial.h"
#include "Material/MyphongInstanceMaterial.h"

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

       /*   m_camera = new OpenGLCamera::MyOrthographicCamera(
              -size, size,
              size, -size,
              size, -size,
              this
          );*/
        m_cameraControl = new OpenGLCamera::MyTrackBallCameraControl(this);
        //m_cameraControl = new OpenGLCamera::MyGameCameraControl(this);
        m_cameraControl->setcamera(m_camera);
        m_cameraControl->setSensitivity(0.4f);
        m_cameraControl->setScaleSpeed(0.01f);

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

    void MyGLWidget::setModelBlend(MyObject* obj, bool blend, float opacity)
    {
        if (obj->getType() == MyOpenGL::ObjectType::Mesh)
        {
            MyMesh* mesh = static_cast<MyMesh*> (obj);
            MyMaterial* mat = mesh->mMaterial;
            mat->mBlend = blend;
            mat->mOpacity = opacity;
            mat->mDepthWrite = false;
        }

        auto children = obj->getChildren();
        for (int i = 0; i < children.size(); i++)
        {
            setModelBlend(children[i], blend, opacity);
        }
    }

    void MyGLWidget::bShowLogo(bool bshow)
    {
        if (logomesh)
        {
            bool flag = logomesh->getShow();
            logomesh->setShow(!flag);
        }
        update();
    }

    void MyGLWidget::bShowMoxing(bool bshow)
    {
        if (moxingmesh)
        {
            bool flag = moxingmesh->getShow();
			moxingmesh->setShow(!flag);
        }
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
        m_inscreenscene = new ::MyOpenGL::MyScene();
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
        auto geo = MyGeometry::createScreenPlane(-1, -0.95, 0.9, 1);
        auto mat = new MyScreenMaterial();
        mat->mScreenTexture = new MyTexture("../assets/textures/logo_left_top.png",0); // 设置屏幕纹理
        mat->mBlend = true;
        logomesh = new MyMesh(geo, mat);

        m_scene->addChild(logomesh);
        
    }

    void MyGLWidget::preparemoxing()
    {
		m_renderer = new::MyOpenGL::MyRenderer();
        m_scene = new ::MyOpenGL::MyScene();
        m_inscreenscene = new ::MyOpenGL::MyScene();
		framebuffer = new Framebuffer(width(), height());

        //preparelogo();

	/*	auto testmodel = MyAssimpLoader::load("../assets/fbx/Fist Fight B.fbx");
        testmodel->setScale(glm::vec3(0.1f));
        if (!testmodel)
            return;
        m_scene->addChild(testmodel);*/

        //shendutest();
        //mobantest();
        //colorblendtest();
        //colorblendshendutest();
        //opacityMaskTest();
        //FaceCullingTest();
        //prepareScreen();
        
        //DoublepassTest();
        //prepareMengGuRen();
        prepareJizuobiao();

        //平行光
		m_dirLight = new::MyOpenGL::MyDirectionalLight();
        m_dirLight->mDirection = glm::vec3(-1.0f);
        m_dirLight->mSpecularIntensity = 0.0f;
        
        // 环境光
		m_ambLight = new ::MyOpenGL::MyAmbientLight(); 
		m_ambLight->mColor = glm::vec3(0.0f); 

        bhasrenderer = true;
    }

    void MyGLWidget::shendutest()
    {
        //深度测试
        auto geometry = MyGeometry::createPlane(5.0f, 5.0f);
        auto materialA = new MyPhongMaterial();
        materialA->mDiffuse = new MyTexture("../assets/textures/goku.jpg", 0); // 兼容原有指针成员
        auto meshA = new::MyOpenGL::MyMesh(geometry, materialA);
        m_scene->addChild(meshA);

        auto materialB = new MyPhongMaterial();
        materialB->mDiffuse = new MyTexture("../assets/textures/box.png", 0); // 兼容原有指针成员
        materialB->mPolygonOffset = true;
        materialB->mFactor = 1.0f; // 设置偏移因子
        materialB->mUnit = 1.0f;


        //materialB->mDepthWrite = false;
        auto meshB = new::MyOpenGL::MyMesh(geometry, materialB);
        meshB->setPosition(glm::vec3(2.0f, 0.5f, -0.000000001f));
        m_scene->addChild(meshB);
    }

    void MyGLWidget::mobantest()
    {
        //1 创建一个普通方块
        auto geometry = MyGeometry::createBox(4.0f);
        auto meterialA = new MyPhongMaterial();
        meterialA->mDiffuse = new MyTexture("../assets/textures/goku.jpg", 0); // 兼容原有指针成员

        auto meshA = new MyMesh(geometry, meterialA);
        m_scene->addChild(meshA);
        //设置模版测试参数
        meterialA->mStencilTest = true;
        meterialA->mSFail = GL_KEEP;
        meterialA->mZFail = GL_KEEP;
        meterialA->mZPass = GL_REPLACE;
        //控制写入
        meterialA->mStencilMask = 0xff;
        //模版参数规则
		meterialA->mStencilFunc = GL_ALWAYS; 
        meterialA->mStencilRef = 1;
        meterialA->mStencilFuncMask = 0xff;

        //2 创建一个勾选方块
        auto meterialbound = new MyWhiteMaterial();
        auto meshbound = new MyMesh(geometry, meterialbound);
        meterialbound->mDepthTest = false;
        meshbound->setPosition(meshA->getPosition());
        meshbound->setScale(glm::vec3(1.01f));
        m_scene->addChild(meshbound);

        //设置模版测试参数
        meterialbound->mStencilTest = true;
        meterialbound->mSFail = GL_KEEP;
        meterialbound->mZFail = GL_KEEP;
        meterialbound->mZPass = GL_KEEP;
        //控制写入
        meterialbound->mStencilMask = 0x00;
        //模版参数规则
        meterialbound->mStencilFunc = GL_NOTEQUAL;
        meterialbound->mStencilRef = 1;
        meterialbound->mStencilFuncMask = 0xff;

        //meshB 创建一个普通方块
        auto geometryB = MyGeometry::createBox(4.0f);
        auto meterialB = new MyPhongMaterial();
        meterialB->mDiffuse = new MyTexture("../assets/textures/wall.jpg", 0); // 兼容原有指针成员
        auto meshB = new MyMesh(geometryB, meterialB);
        meshB->setPosition(glm::vec3(3.0f, 1.0f, 1.0f));
        m_scene->addChild(meshB);
        //设置模版测试参数
        meterialB->mStencilTest = true;
        meterialB->mSFail = GL_KEEP;
        meterialB->mZFail = GL_KEEP;
        meterialB->mZPass = GL_REPLACE;
        //控制写入
        meterialB->mStencilMask = 0xff;
        //模版参数规则
        meterialB->mStencilFunc = GL_ALWAYS;
        meterialB->mStencilRef = 1;
        meterialB->mStencilFuncMask = 0xff;

        //meshB 创建一个勾选方块
        auto meterialBbound = new MyWhiteMaterial();
        auto meshBbound = new MyMesh(geometryB, meterialBbound);
        meterialBbound->mDepthTest = false;
        meshBbound->setPosition(meshB->getPosition());
        meshBbound->setScale(glm::vec3(1.01f));
        m_scene->addChild(meshBbound);

        //设置模版测试参数
        meterialBbound->mStencilTest = true;
        meterialBbound->mSFail = GL_KEEP;
        meterialBbound->mZFail = GL_KEEP;
        meterialBbound->mZPass = GL_KEEP;
        //控制写入
        meterialBbound->mStencilMask = 0x00;
        //模版参数规则
        meterialBbound->mStencilFunc = GL_NOTEQUAL;
        meterialBbound->mStencilRef = 1;
        meterialBbound->mStencilFuncMask = 0xff;
    }

    void MyGLWidget::colorblendtest()
    {
        //box 
        auto boxgeo = MyGeometry::createBox(4.0);
        auto boxmat = new MyPhongMaterial();
        boxmat->mDiffuse =new MyTexture("../assets/textures/box.png", 0);

        auto boxmesh = new MyMesh(boxgeo, boxmat);
        m_scene->addChild(boxmesh);

        //plane
        auto planegeo = MyGeometry::createPlane(6.0, 6.0);
        auto planemat = new MyPhongMaterial();
        planemat->mDiffuse = new MyTexture("../assets/textures/window.png", 0);

        auto planemesh = new MyMesh(planegeo, planemat);
        planemesh->setPosition(glm::vec3(0.0, 0.0, 4.0));
        m_scene->addChild(planemesh);
    }

    void MyGLWidget::colorblendshendutest()
    {
        //背包
        auto bagmodel = MyAssimpLoader::load("../assets/fbx/bag/backpack.obj");
        setModelBlend(bagmodel, true, 0.5);
        
        bagmodel->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
        m_scene->addChild(bagmodel);

        //白色盒子
		/*auto boxgeo = MyGeometry::createBox(4.0);
        auto boxmat = new MyWhiteMaterial();

        auto boxmesh = new MyMesh(boxgeo, boxmat);
        m_scene->addChild(boxmesh);*/

        //盒子
  //      auto boxgeo = MyGeometry::createBox(4.0);
  //      auto boxmat = new MyPhongMaterial();
		//boxmat->mDiffuse = new MyTexture("../assets/textures/window.png", 0); // 兼容原有指针成员
		//boxmat->mBlend = true; // 开启混合
  //      boxmat->mOpacity = 1;
  //      boxmat->mDepthWrite = false;
  //      auto boxmesh = new MyMesh(boxgeo, boxmat);
  //      
  //       m_scene->addChild(boxmesh);
    }

    void MyGLWidget::opacityMaskTest()
    {
		auto planegeo = MyGeometry::createPlane(5.0, 5.0);
		auto planemat = new MyOpacityMaskMaterial();
		planemat->mDiffuse = new MyTexture("../assets/textures/grass.jpg", 0); 
		planemat->mOpacityMask = new MyTexture("../assets/textures/grassMask.png", 1); 
		planemat->mBlend = true; // 开启混合

		auto planemesh = new MyMesh(planegeo, planemat);
		
		m_scene->addChild(planemesh);


    
    }

    void MyGLWidget::FaceCullingTest()
    {
        auto planegeo = MyGeometry::createPlane(5.0, 5.0);
        auto planemat = new MyPhongMaterial();
        planemat->mDiffuse = new MyTexture("../assets/textures/grass.jpg", 0);
		planemat->mFaceCulling = true; // 开启面剔除
		planemat->mFrontFace = GL_CCW; // 设置前面为逆时针
        planemat->mCullFace = GL_BACK;

        auto planemesh = new MyMesh(planegeo, planemat);

        m_scene->addChild(planemesh);
    }

    void MyGLWidget::DoublepassTest()
    {
        //离屏渲染的box
        auto boxgeo = MyGeometry::createBox(5.0f);
        auto boxmat = new MyPhongMaterial();
        boxmat->mDiffuse = new MyTexture("../assets/textures/grass.jpg", 0);

        auto boxmesh = new MyMesh(boxgeo, boxmat);
		m_scene->addChild(boxmesh);

        //贴到屏幕上的矩形
        auto geo = MyGeometry::createScreenPlane(-1,1,-1,1);
        auto mat = new MyScreenMaterial();
        if(framebuffer)
            mat->mScreenTexture = framebuffer->mColorAttachment;//!!!!重要!!!!!

        auto mesh = new MyMesh(geo, mat);
        if(m_inscreenscene)
            m_inscreenscene->addChild(mesh);
        
    }

  

    void MyGLWidget::prepareGrass()
    {
		auto grassmodel = MyAssimpLoader::load("../assets/fbx/grass.fbx");
		grassmodel->setScale(glm::vec3(0.02f));
		m_scene->addChild(grassmodel);

		auto grassmat = new MyOpacityMaskMaterial();
		grassmat->mDiffuse = new MyTexture("../assets/textures/grass.jpg", 0);
		grassmat->mOpacityMask = new MyTexture("../assets/textures/grassMask.png", 1);
		grassmat->mBlend = true; // 开启混合
		grassmat->mDepthWrite = false; // 禁用深度写入
		m_renderer->mGlobalMaterial = grassmat; // 设置全局材质
    }

    void MyGLWidget::prepareScreen()
    {
		auto geo = MyGeometry::createScreenPlane(-1,1,-1,1);
		auto mat = new MyScreenMaterial();
		mat->mScreenTexture = new MyTexture("../assets/textures/box.png", 0); // 设置屏幕纹理
        
		moxingmesh = new MyMesh(geo, mat);
		m_scene->addChild(moxingmesh);
    }

    void MyGLWidget::prepareMengGuRen()
    {
        std::vector<std::string> paths = {
            "../assets/textures/skybox/right.jpg",
            "../assets/textures/skybox/left.jpg",
            "../assets/textures/skybox/top.jpg",
            "../assets/textures/skybox/bottom.jpg",
            "../assets/textures/skybox/back.jpg",
            "../assets/textures/skybox/front.jpg",
		};
        MyTexture* envTexBox = new MyTexture(paths, 0);
        auto boxgeo = MyGeometry::createBox(1.0f);
        auto boxmat = new MyCubeMaterial();
        boxmat->mDiffuse = envTexBox; // 设置屏幕纹理
        boxmat->mDepthWrite = false;
        moxingmesh = new MyMesh(boxgeo, boxmat);
        m_scene->addChild(moxingmesh);

        MyTexture* envTex = new MyTexture(paths, 1);
        auto spheregeo = MyGeometry::createSphere(4.0f);
        auto spheremat = new MyPhongEnvMaterial();
        spheremat->mDiffuse = new MyTexture("../assets/textures/earth.jpg", 0); // 设置屏幕纹理
        spheremat->mEnv = envTex;
        auto spheremesh = new MyMesh(spheregeo, spheremat);
        m_scene->addChild(spheremesh);

		

       
    }

    void MyGLWidget::prepareJizuobiao()
    {
    
        auto boxgeo = MyGeometry::createBox(1.0f);
        auto boxmat = new MyCubeMaterial();
        boxmat->mDiffuse = new MyTexture("../assets/textures/bk.jpg", 0);
        moxingmesh = new MyMesh(boxgeo, boxmat);
        m_scene->addChild(moxingmesh);

        auto spheregeo = MyGeometry::createSphere(4.0f);
        auto spheremat = new MyPhongInstanceMaterial();
        spheremat->mDiffuse = new MyTexture("../assets/textures/earth.jpg", 0); // 设置屏幕纹理
        auto spheremesh = new MyInstancedMesh(spheregeo, spheremat,3);
        glm::mat4 transform0 = glm::mat4(1.0f);
		glm::mat4 transform1 = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
        glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 8.0f, 0.0f));
        spheremesh->mInstanceMatrices[0] = transform0;
		spheremesh->mInstanceMatrices[1] = transform1;
        spheremesh->mInstanceMatrices[2] = transform2;
    
        spheremesh->updateMatrices();
        m_scene->addChild(spheremesh);
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
            //更新窗口大小数据
			m_renderer->setWidth(width());
            m_renderer->setHeight(height());
            // pass01 将box渲染到colorAttachment上，新的fbo上 
            m_renderer->render(m_scene, m_camera, m_dirLight, m_pointLights, m_spotLight, m_ambLight,framebuffer->mFBO);
          

            
            //if (m_inscreenscene)
            //{
            //    // pass02 将colorAttachment作为纹理，绘制到整个屏幕上
            //    m_renderer->render(m_inscreenscene, m_camera, m_dirLight, m_pointLights, m_spotLight, m_ambLight,defaultFramebufferObject());
            //}
           
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
   
        
        preparemoxing();
        

      
        prepareCamera();

        m_prepared = true;
        emit prepareok(true);
        doneCurrent();
       
    }
   

}