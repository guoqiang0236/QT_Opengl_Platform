#pragma once
#include "MyCameraControl.h"
namespace OpenGLCamera {
	class MyGameCameraControl : public MyCameraControl
	{
	public:
		MyGameCameraControl(QObject* parent = nullptr);
		~MyGameCameraControl();

		// ��д�����麯��
		void onCursor(double xpos, double ypos) override;
		void update() override;

		// �ٶ����ýӿ�
		void setSpeed(float s) { mSpeed = s; }

	private:
		// �ڲ���ת����
		void rotatePitch(float angle);
		void rotateYaw(float angle);

	private:
		// ��Ա�������ϸ�ͼƬ��ʼ����
		float mPitch{ 0.0f };  // ��ǰ������
		float mSpeed{ 0.1f };  // Ĭ���ƶ��ٶ�
	};
}