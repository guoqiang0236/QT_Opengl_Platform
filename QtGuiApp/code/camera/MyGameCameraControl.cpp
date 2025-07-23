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
        // 0. 判断
        mPitch += angle;
        if (mPitch > 89.0f || mPitch < -89.0f) // 限制俯仰角
        {
            mPitch -= angle;
            return;
        }
        // 1. 绕着mRight向量旋转
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);

        // 2. 影响当前相机的up向量
        mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f); // vec4给到vec3，自动取xyz 

    }

    void MyGameCameraControl::rotateYaw(float angle)
    {
        //绕着世界坐标系的Y轴旋转
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        // 2. 影响当前相机的up向量、right向量
        mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f); // vec4给到vec3，自动取xyz
        mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f); // vec4给到vec3，自动取xyz

    }

    void MyGameCameraControl::update()
    {
        // 1. 计算相机前方向量（右手坐标系）
        auto front = glm::cross(mCamera->mUp, mCamera->mRight);
        auto right = mCamera->mRight;  // 直接获取相机右向量

        // 2. 初始化移动方向向量
        glm::vec3 direction(0.0f);

        // 3. 处理键盘输入（Qt 方式）
        if (mKeyMap[Qt::Key_W]) {
            direction += front;  // 前进
        }
        if (mKeyMap[Qt::Key_S]) {
            direction -= front;  // 后退
        }
        if (mKeyMap[Qt::Key_A]) {
            direction += right;  // 左移
        }
        if (mKeyMap[Qt::Key_D]) {
            direction -= right;  // 右移
        }

        // 4. 标准化并应用移动（图片中未展示的后续处理）
        if (glm::length(direction) != 0.0f) {
            direction = glm::normalize(direction);
            mCamera->mPosition += direction * mSpeed;
        }
    }

}
