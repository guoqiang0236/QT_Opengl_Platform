#include "MyTrackBallCameraControl.h"


MyTrackBallCameraControl::MyTrackBallCameraControl(QObject* parent)
{
}

MyTrackBallCameraControl::~MyTrackBallCameraControl()
{
}

void MyTrackBallCameraControl::onCursor(double xpos, double ypos)
{
    // 当左键按下时处理相机旋转
    if (mLeftMouseDown)
    {
        // 调整相机的各类参数
        // 1. 计算经线跟纬线旋转的增量角度(正负都有可能)
        float deltaX = static_cast<float>(xpos - mCurrentX) * mSensitivity;
        float deltaY = static_cast<float>(ypos - mCurrentY) * mSensitivity;

        // 2. 分开pitch跟yaw各自计算
        // （图片中未展示具体实现，此处添加典型处理逻辑）
        rotatePitch(-deltaY);  // 俯仰角（上下旋转）
        rotateYaw(-deltaX);    // 偏航角（左右旋转）
    }
	else if (mMiddleMouseDown)
	{
        // 1. 计算经线跟纬线平移的增量(正负都有可能)
        float deltaX = static_cast<float>(xpos - mCurrentX) * mMoveSpeed;
        float deltaY = static_cast<float>(ypos - mCurrentY) * mMoveSpeed;

		mCamera->mPosition += mCamera->mUp * deltaY; // 上下平移
		mCamera->mPosition -= mCamera->mRight * deltaX; // 左右平移
	}

    // 更新当前鼠标位置（无论是否按下都执行）
    mCurrentX = xpos;
    mCurrentY = ypos;
}

void MyTrackBallCameraControl::onScroll(double offset)
{
	mCamera->scale(offset * mScaleSpeed); // 缩放相机
}

void MyTrackBallCameraControl::rotatePitch(float angle)
{
    // 1. 绕着mRight向量旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);

    // 2. 影响当前相机的up向量和位置
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f); // vec4给到vec3，自动取xyz
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void MyTrackBallCameraControl::rotateYaw(float angle)
{
    //绕着世界坐标系的Y轴旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    // 2. 影响当前相机的up向量、right向量和位置
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f); // vec4给到vec3，自动取xyz
    mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}
