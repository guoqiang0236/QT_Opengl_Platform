#pragma once
#include "MyLight.h"
#include "../MyObject.h"

class MyPointLight : public MyLight, public MyObject
{
public:
	MyPointLight();
	~MyPointLight();

	float mK1 = 1.0f;
	float mK2 = 1.0f;
	float mKc = 1.0f;

private:

};

