#include "vaoopenglwidget.h"


GLuint  VAO, VBO;

float vertices[] = {
    -0.5f,-0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    0.0f,0.5f,0.0f,
};


VAOOpenGLWidget::VAOOpenGLWidget(QWidget *parent):QOpenGLWidget(parent)
{

}

void VAOOpenGLWidget::initializeGL()
{
    //step 1.初始化，指向OpenGL函数.
    initializeOpenGLFunctions();

    //step 2. create VAO&VBO.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //step 3. bind VAO&VBO.
    glBindVertexArray(VAO);
    glBindBuffer( GL_ARRAY_BUFFER, VBO );

    //step 4. 把顶点数据绑定到目标上(你这样理解,把数据CPU->GPU上保存,方便接下来快速绘制)
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW ); //

    //step 5. 告诉显卡如何解析.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    //step 6. 开启VAO的第一个属性值.
    glEnableVertexAttribArray(0);

    //step 7. 释放.
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}


void VAOOpenGLWidget::resizeGL(int w, int h)
{

}

void VAOOpenGLWidget::paintGL()
{
    glClearColor( 0.1f,0.1f,0.1f,1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    glBindVertexArray(VAO);

    glDrawArrays( GL_TRIANGLES, 0, 3 );

    glBindVertexArray(0);
}