#include "MyOrthographicCamera.h"



MyOrthographicCamera::MyOrthographicCamera(float l, float r, float t, float b, float n, float f)
{
	mLeft = l;
	mRight = r;
	mTop = t;
	mBottom = b;
	mNear = n;
	mFar = f;
}

MyOrthographicCamera::~MyOrthographicCamera()
{
}

glm::mat4 MyOrthographicCamera::getProjectionMatrix()
{
	return glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
}


