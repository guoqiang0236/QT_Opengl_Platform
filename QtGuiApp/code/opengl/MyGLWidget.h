#pragma once
#include "../pch.h"
#include "MyShader.h"
#include "MyTexture.h"
#include "MyGeometry.h"

//引入相机+控制器
#include "../camera/MyOrthographicCamera.h"
#include "../camera/MyPerspectiveCamera.h"
#include "../camera/MyTrackBallCameraControl.h"
#include "../camera/MyGameCameraControl.h"


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget* parent = nullptr);
    ~MyGLWidget();

private:
    // 学习用的功能函数
    void prapareBackground();
    void prepareShader();
    void preparecolortriangleShader();
    void prepareVBO();
    void prepareEBOVAO();
	void prepareVAOcolortriangle();
    void prepareVAOForGLTriangles();
    void prepare();
    void prepareSingleBuffer();
    void prepareInterleaveBuffer();
  
	//SHADER
    void prepareShaderPtr();
	void prepareShaderPtrForMat();
	void prepareShaderPtrForGeometry();

    //VAO
	void prepareVAOForGLTrianglesWithTexture();
    void prepareVAOForTexture();
    void prepareVAOForMipmapTexture();
    void prepareVAOForLiuYiFei();
	void prepareVAOForGeometry();

	//TEXTURE
    void prepareTexture();
    void prepareTexturePtr();
    void prepareMipmapTexturePtr();
    void prepareMixTexturePtr();
    void prepareMipmapLiuYiFeiTexturePtr();
	
    

    //渲染
    void prepareStates();
	void prepareCamera();// 准备摄像机
	void prepareOrtho();// 准备正交投影
	void preparareProjection();// 准备透视投影
    void render();


    //图形变换
	void doRotationTransform();//旋转
    void doTranslationTransform();//平移变换
    void doScaleTransform(); //缩放变换
    void doTransform();//复合变换
    void doRotation();//持续旋转
    void doTransformDieJia();//叠加变换(本地坐标系)
    void preTransformDieJia();//叠加预先变换(本地坐标系)
public slots:
    void triggerDrawTriangle(); // 供按钮点击触发
	void triggerDrawTexture(); 
    void triggerDrawMixTexture();
    void triggerDrawMipmapTexture();
    void triggerDrawLiuYiFei();

	void triggerDrawTwoPictures();

	void triggerDrawGeometryBox();



protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;                // 对应帧缓冲区大小变化
    void keyPressEvent(QKeyEvent* event) override;       // 对应键盘按下
	void keyReleaseEvent(QKeyEvent* event) override;     // 对应键盘释放
	void mousePressEvent(QMouseEvent* event) override;   // 对应鼠标按下
	void mouseReleaseEvent(QMouseEvent* event) override; // 对应鼠标释放
	void mouseMoveEvent(QMouseEvent* event) override;    // 对应鼠标移动
    void wheelEvent(QWheelEvent* event)override; 
	void showEvent(QShowEvent* event) override; // 对应窗口显示

private:
    MyShader* getShader() const;
    GLuint getProgram() const;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLuint m_program = 0;
    GLuint m_texture = 0;
    bool m_prepared = false;
	std::unique_ptr<MyShader> m_Shader;
	std::unique_ptr<MyTexture> m_Texture;
    std::unique_ptr<MyTexture> m_Texture2;
    MyTexture* grassTexture = nullptr;
    MyTexture* landTexture = nullptr;
    MyTexture* noiseTexture = nullptr;
    QElapsedTimer m_timer;
	glm::mat4 m_transform;// 变换矩阵
    glm::mat4 m_transform2;// 变换矩阵
	glm::mat4 m_viewMatrix;// 摄像机矩阵
	glm::mat4 m_projectionMatrix; // 投影矩阵
    //平行光
	glm::vec3 m_lightDirection{ -0.4f, -1.4f, -1.9f }; // 平行光方向
	glm::vec3 m_lightColor{ 1.0f, 1.0f, 1.0f }; // 平行光颜色
    float m_angle;

    MyCamera* m_camera = nullptr; // 透视摄像机
    MyCameraControl* m_cameraControl = nullptr; // 摄像机控制器
    MyGeometry* m_geometry = nullptr;
  
};

