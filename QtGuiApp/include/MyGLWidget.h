#pragma once
#include "pch.h"
#include "MyShader.h"
#include "MyTexture.h"

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
  
    void prepareShaderPtr();
	void prepareShaderPtrForMat();
	void prepareTexture();
	void prepareVAOForGLTrianglesWithTexture();
    void prepareVAOForTexture();
    void prepareVAOForMipmapTexture();
    void prepareVAOForLiuYiFei();
    void prepareTexturePtr();
    void prepareMipmapTexturePtr();
    void prepareMixTexturePtr();
    void prepareMipmapLiuYiFeiTexturePtr();

    //渲染
    void prepareCamera();
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



protected:
    void initializeGL() override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeGL(int w, int h) override;
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
    MyTexture* grassTexture = nullptr;
    MyTexture* landTexture = nullptr;
    MyTexture* noiseTexture = nullptr;
    QElapsedTimer m_timer;
    glm::mat4 m_transform;
	glm::mat4 m_viewmatrix;
    float m_angle;
  
};

