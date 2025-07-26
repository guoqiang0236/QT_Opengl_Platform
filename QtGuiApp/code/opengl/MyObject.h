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
        // ���캯��
        MyObject();
        ~MyObject();

        // ��������λ��
        void setPosition(glm::vec3 pos);

        // ������ת����
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);

        // ���ýǶȷ���
        void setAngleX(float angle);
        void setAngleY(float angle);
        void setAngleZ(float angle);

        // �������ű���
        void setScale(glm::vec3 scale);

        // ��ȡģ�ͱ任����
        glm::mat4 getModelMatrix();

        glm::vec3 getPosition()const { return m_Position; }

        //���ӹ�ϵ
        void addChild(MyObject* obj);             // ����Ӷ���
        std::vector<MyObject*> getChildren();     // ��ȡ�����Ӷ���
        MyObject* getParent();                    // ��ȡ������ָ��

        ObjectType getType()const { return m_Type; }

    protected:
        // ��Ա���������ͼ��ȫһ�£�
        glm::vec3 m_Position{ 0.0f };  // ����λ��


        // unity��ת��׼��pitch yaw roll
        float m_AngleX{ 0.0f };        // X����ת�Ƕȣ����ȣ�
        float m_AngleY{ 0.0f };        // Y����ת�Ƕȣ����ȣ�
        float m_AngleZ{ 0.0f };        // Z����ת�Ƕȣ����ȣ�


        glm::vec3 m_Scale{ 0.1f };

        //���ӹ�ϵ
        std::vector<MyObject*> m_Children; // �������б�
        MyObject* m_Parent{ nullptr }; // ������ָ��

        //���ͼ�¼
        ObjectType m_Type;

    };
}