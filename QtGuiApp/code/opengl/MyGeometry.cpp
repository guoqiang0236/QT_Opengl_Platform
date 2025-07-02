#include "MyGeometry.h"

MyGeometry::MyGeometry()
{
	initializeOpenGLFunctions(); // Ensure OpenGL functions are initialized

}

MyGeometry::~MyGeometry()
{
	if (mVao != 0)
	{
		glDeleteVertexArrays(1, &mVao);
		mVao = 0;
	}
	if (mPosVbo != 0)
	{
		glDeleteBuffers(1, &mPosVbo);
		mPosVbo = 0;
	}
	if (mUvVao != 0)
	{
		glDeleteBuffers(1, &mUvVao);
		mUvVao = 0;
	}
	if (mEbo != 0)
	{
		glDeleteBuffers(1, &mEbo);
		mEbo = 0;
	}
}

MyGeometry* MyGeometry::createBox(float size)
{
	MyGeometry* geometry = new MyGeometry();

	return geometry;
}

MyGeometry* MyGeometry::createSphere(float size)
{
	MyGeometry* geometry = new MyGeometry();

	return geometry;
}
