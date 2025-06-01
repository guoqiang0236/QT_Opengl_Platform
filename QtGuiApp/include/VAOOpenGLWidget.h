#ifndef VAOOPENGLWIDGET_H
#define VAOOPENGLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class VAOOpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
public:
    VAOOpenGLWidget(QWidget *parent = nullptr);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // VAOOPENGLWIDGET_H