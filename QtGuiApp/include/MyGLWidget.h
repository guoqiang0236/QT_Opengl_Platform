#pragma once

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core> // 添加此头文件
#include <QObject>
#include <QWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget* parent = nullptr);
    ~MyGLWidget();

    // 学习用的功能函数
    void prapareBackground();
    void prepareShader();
    void prepareVBO();
    void prepare();
    void prepareSingleBuffer();
    void prepareInterleaveBuffer();
    void render();

public slots:
    void triggerDrawTriangle(); // 供按钮点击触发

protected:
    void initializeGL() override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeGL(int w, int h) override;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_program = 0;
    bool m_prepared = false;
};

