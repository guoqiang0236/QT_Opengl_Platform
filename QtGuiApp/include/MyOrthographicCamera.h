#pragma once
#include "MyCamera.h"


class MyOrthographicCamera : public MyCamera
{
public:
	MyOrthographicCamera(float l,float r,float t,float b ,float n,float f);
	~MyOrthographicCamera();


    glm::mat4 getProjectionMatrix()  override;

private:
    float mLeft = 0.0f;
    float mRight = 0.0f;
    float mTop = 0.0f;
    float mBottom = 0.0f;
    float mNear = 0.0f;
    float mFar = 0.0f;
};