#pragma once
#include "MyLight.h"
#include "../MyObject.h"

class MySpotLight : public MyLight, public MyObject
{
public:
	MySpotLight();
	~MySpotLight();

public:
	glm::vec3 mTargetDirection = glm::vec3(-1.0f, -1.0f, -1.0f);
	float mVisibleAngle = 0.0f;
};

