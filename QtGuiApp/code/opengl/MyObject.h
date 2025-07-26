#pragma once

#include "../pch.h"
#include <glm/glm.hpp>
namespace MyOpenGL {
    enum class ObjectType {
        Object,
        Mesh,
        Scene
    };
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

        // 设置角度方法
        void setAngleX(float angle);
        void setAngleY(float angle);
        void setAngleZ(float angle);

        // 设置缩放比例
        void setScale(glm::vec3 scale);

        // 获取模型变换矩阵
        glm::mat4 getModelMatrix();

        glm::vec3 getPosition()const { return m_Position; }

        //父子关系
        void addChild(MyObject* obj);             // 添加子对象
        std::vector<MyObject*> getChildren();     // 获取所有子对象
        MyObject* getParent();                    // 获取父对象指针

        ObjectType getType()const { return m_Type; }

    protected:
        // 成员变量（与截图完全一致）
        glm::vec3 m_Position{ 0.0f };  // 物体位置


        // unity旋转标准：pitch yaw roll
        float m_AngleX{ 0.0f };        // X轴旋转角度（弧度）
        float m_AngleY{ 0.0f };        // Y轴旋转角度（弧度）
        float m_AngleZ{ 0.0f };        // Z轴旋转角度（弧度）


        glm::vec3 m_Scale{ 0.1f };

        //父子关系
        std::vector<MyObject*> m_Children; // 子物体列表
        MyObject* m_Parent{ nullptr }; // 父物体指针

        //类型记录
        ObjectType m_Type;

    };
}