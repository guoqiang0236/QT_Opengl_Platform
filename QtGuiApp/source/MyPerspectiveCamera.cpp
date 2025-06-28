#include "MyPerspectiveCamera.h"

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
	return glm::perspective(glm::radians(mFovy),mAspect,mNear,mFar);
}
