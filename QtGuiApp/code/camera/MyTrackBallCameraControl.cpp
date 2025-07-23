#include "MyTrackBallCameraControl.h"

namespace OpenGLCamera {
    MyTrackBallCameraControl::MyTrackBallCameraControl(QObject* parent)
    {
    }

    MyTrackBallCameraControl::~MyTrackBallCameraControl()
    {
    }

    void MyTrackBallCameraControl::onCursor(double xpos, double ypos)
    {
        // ���������ʱ���������ת
        if (mLeftMouseDown)
        {
            // ��������ĸ������
            // 1. ���㾭�߸�γ����ת�������Ƕ�(�������п���)
            float deltaX = static_cast<float>(xpos - mCurrentX) * mSensitivity;
            float deltaY = static_cast<float>(ypos - mCurrentY) * mSensitivity;

            // 2. �ֿ�pitch��yaw���Լ���
            // ��ͼƬ��δչʾ����ʵ�֣��˴���ӵ��ʹ����߼���
            rotatePitch(-deltaY);  // �����ǣ�������ת��
            rotateYaw(-deltaX);    // ƫ���ǣ�������ת��
        }
        else if (mMiddleMouseDown)
        {
            // 1. ���㾭�߸�γ��ƽ�Ƶ�����(�������п���)
            float deltaX = static_cast<float>(xpos - mCurrentX) * mMoveSpeed;
            float deltaY = static_cast<float>(ypos - mCurrentY) * mMoveSpeed;

            mCamera->mPosition += mCamera->mUp * deltaY; // ����ƽ��
            mCamera->mPosition -= mCamera->mRight * deltaX; // ����ƽ��
        }

        // ���µ�ǰ���λ�ã������Ƿ��¶�ִ�У�
        mCurrentX = xpos;
        mCurrentY = ypos;
    }

    void MyTrackBallCameraControl::onScroll(double offset)
    {
        mCamera->scale(offset * mScaleSpeed); // �������
    }

    void MyTrackBallCameraControl::rotatePitch(float angle)
    {
        // 1. ����mRight������ת
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);

        // 2. Ӱ�쵱ǰ�����up������λ��
        mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f); // vec4����vec3���Զ�ȡxyz
        mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
    }

    void MyTrackBallCameraControl::rotateYaw(float angle)
    {
        //������������ϵ��Y����ת
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        // 2. Ӱ�쵱ǰ�����up������right������λ��
        mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f); // vec4����vec3���Զ�ȡxyz
        mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
        mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
    }
}