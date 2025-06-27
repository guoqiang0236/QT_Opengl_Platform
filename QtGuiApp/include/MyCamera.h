#pragma once

#include"pch.h"

class MyCamera
{
public:
	MyCamera();
	~MyCamera();

	glm::mat4 getViewMatrix();
public:
	glm::vec3 mPosition{ 0.0f,0.0f,1.0f };
	glm::vec3 mUp{ 0.0f,1.0f,0.0f };
	glm::vec3 mRight{ 1.0f, 0.0f, 0.0f };

};
