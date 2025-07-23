#pragma once
#include "MyLight.h"
#include "../MyObject.h"
namespace MyOpenGL {
	class MyPointLight : public MyLight, public MyOpenGL::MyObject
	{
	public:
		MyPointLight();
		~MyPointLight();

		float mK1 = 1.0f;
		float mK2 = 1.0f;
		float mKc = 1.0f;

	private:

	};

}