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
		//传入的fov是角度，需要转换为弧度
		return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
	}

	void MyPerspectiveCamera::scale(float deltaScale)
	{
		auto front = glm::cross(mUp, mRight);
		mPosition += front * deltaScale; // 沿着前方向量移动
	}
}