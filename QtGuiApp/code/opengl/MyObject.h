#pragma once

#include "../pch.h"
#include <glm/glm.hpp>

class MyObject :public QObject, protected QOpenGLFunctions_4_5_Core {
    Q_OBJECT
public:
    // 构造函数
    MyObject();
    ~MyObject();

    // 设置物体位置
    void setPosition(glm::vec3 pos);

    // 增量旋转方法
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    // 设置缩放比例
    void setScale(glm::vec3 scale);

    // 获取模型变换矩阵
    glm::mat4 getModelMatrix() ;


protected:
    // 成员变量（与截图完全一致）
    glm::vec3 m_Position{ 0.0f };  // 物体位置


    // unity旋转标准：pitch yaw roll
    float m_AngleX{ 0.0f };        // X轴旋转角度（弧度）
    float m_AngleY{ 0.0f };        // Y轴旋转角度（弧度）
    float m_AngleZ{ 0.0f };        // Z轴旋转角度（弧度）


    glm::vec3 m_Scale{ 1.0f };
};