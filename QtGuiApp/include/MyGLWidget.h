#pragma once
#include "pch.h"
#include "MyShader.h"

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
	void prepareTexture();
	void prepareVAOForGLTrianglesWithTexture();
    void prepareVAOForTexture();
    

    //渲染
    void render();

public slots:
    void triggerDrawTriangle(); // 供按钮点击触发
	void triggerDrawTexture(); // 供按钮点击触发

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
    QElapsedTimer m_timer;
};

