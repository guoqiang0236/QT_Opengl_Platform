#pragma once

#include"pch.h"

class MyCamera : public QObject
{
	Q_OBJECT
public:
	MyCamera(QObject* parent = nullptr);
	virtual ~MyCamera();
	glm::mat4 getViewMatrix();
	virtual glm::mat4 getProjectionMatrix();
public:
	glm::vec3 mPosition{ 0.0f,0.0f,5.0f };
	glm::vec3 mUp{ 0.0f,1.0f,0.0f };
	glm::vec3 mRight{ 1.0f, 0.0f, 0.0f };

};
