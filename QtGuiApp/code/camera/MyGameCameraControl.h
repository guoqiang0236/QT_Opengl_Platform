#pragma once
#include "MyCameraControl.h"
namespace OpenGLCamera {
	class MyGameCameraControl : public MyCameraControl
	{
	public:
		MyGameCameraControl(QObject* parent = nullptr);
		~MyGameCameraControl();

		// 重写父类虚函数
		void onCursor(double xpos, double ypos) override;
		void update() override;

		// 速度设置接口
		void setSpeed(float s) { mSpeed = s; }

	private:
		// 内部旋转控制
		void rotatePitch(float angle);
		void rotateYaw(float angle);

	private:
		// 成员变量（严格按图片初始化）
		float mPitch{ 0.0f };  // 当前俯仰角
		float mSpeed{ 0.1f };  // 默认移动速度
	};
}