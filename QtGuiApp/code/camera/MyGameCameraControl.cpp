#include "MyGameCameraControl.h"
namespace OpenGLCamera {
    MyGameCameraControl::MyGameCameraControl(QObject* parent)
    {
    }

    MyGameCameraControl::~MyGameCameraControl()
    {
    }

    void MyGameCameraControl::onCursor(double xpos, double ypos)
    {
        float deltaX = static_cast<float>(xpos - mCurrentX);
        float deltaY = static_cast<float>(ypos - mCurrentY);

        if (mLeftMouseDown)
        {
            rotatePitch(deltaY);
            rotateYaw(deltaX);
        }
        mCurrentX = xpos;
        mCurrentY = ypos;
    }


    void MyGameCameraControl::rotatePitch(float angle)
    {
        // 0. �ж�
        mPitch += angle;
        if (mPitch > 89.0f || mPitch < -89.0f) // ���Ƹ�����
        {
            mPitch -= angle;
            return;
        }
        // 1. ����mRight������ת
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);

        // 2. Ӱ�쵱ǰ�����up����
        mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f); // vec4����vec3���Զ�ȡxyz 

    }

    void MyGameCameraControl::rotateYaw(float angle)
    {
        //������������ϵ��Y����ת
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        // 2. Ӱ�쵱ǰ�����up������right����
        mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f); // vec4����vec3���Զ�ȡxyz
        mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f); // vec4����vec3���Զ�ȡxyz

    }

    void MyGameCameraControl::update()
    {
        // 1. �������ǰ����������������ϵ��
        auto front = glm::cross(mCamera->mUp, mCamera->mRight);
        auto right = mCamera->mRight;  // ֱ�ӻ�ȡ���������

        // 2. ��ʼ���ƶ���������
        glm::vec3 direction(0.0f);

        // 3. ����������루Qt ��ʽ��
        if (mKeyMap[Qt::Key_W]) {
            direction += front;  // ǰ��
        }
        if (mKeyMap[Qt::Key_S]) {
            direction -= front;  // ����
        }
        if (mKeyMap[Qt::Key_A]) {
            direction += right;  // ����
        }
        if (mKeyMap[Qt::Key_D]) {
            direction -= right;  // ����
        }

        // 4. ��׼����Ӧ���ƶ���ͼƬ��δչʾ�ĺ�������
        if (glm::length(direction) != 0.0f) {
            direction = glm::normalize(direction);
            mCamera->mPosition += direction * mSpeed;
        }
    }

}
