#include "MyOrthographicCamera.h"




namespace OpenGLCamera {
	MyOrthographicCamera::MyOrthographicCamera(float l, float r, float t, float b, float n, float f, QObject* parent)
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
		float scale = std::pow(2.0f, mScaleFactor); // 使用2的幂次方来计算缩放比例
		return glm::ortho(mLeft * scale, mRight * scale, mBottom * scale, mTop * scale, mNear, mFar);
	}

	void MyOrthographicCamera::scale(float deltaScale)
	{
		mScaleFactor += deltaScale;
	}
}

