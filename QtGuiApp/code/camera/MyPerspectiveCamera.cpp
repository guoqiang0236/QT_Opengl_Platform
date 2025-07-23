#include "MyPerspectiveCamera.h"
namespace OpenGLCamera {
	MyPerspectiveCamera::MyPerspectiveCamera(float fovy, float aspect, float near, float far, QObject* parent)
		: MyCamera(parent)
	{
		mFovy = fovy;
		mAspect = aspect;
		mNear = near;
		mFar = far;
	}

	MyPerspectiveCamera::~MyPerspectiveCamera()
	{
	}

	glm::mat4 MyPerspectiveCamera::getProjectionMatrix()
	{
		//�����fov�ǽǶȣ���Ҫת��Ϊ����
		return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
	}

	void MyPerspectiveCamera::scale(float deltaScale)
	{
		auto front = glm::cross(mUp, mRight);
		mPosition += front * deltaScale; // ����ǰ�������ƶ�
	}
}