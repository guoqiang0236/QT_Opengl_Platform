#pragma once
#include "MyLight.h"
#include "../MyObject.h"
namespace MyOpenGL {
	class MySpotLight : public MyLight, public MyOpenGL::MyObject
	{
	public:
		MySpotLight();
		~MySpotLight();

	public:
		glm::vec3 mTargetDirection = glm::vec3(-1.0f, -1.0f, -1.0f);// ����ķ���
		float mInnerAngle{ 0.0f }; //�ڲ��Ƕ�
		float mOuterAngle{ 0.0f }; //�ⲿ�Ƕ� 

	};
}
