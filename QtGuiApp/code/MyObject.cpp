#include "MyObject.h"

MyObject::MyObject()
{
}

MyObject::~MyObject()
{
}

void MyObject::setPosition(glm::vec3 pos)
{
	m_Position = pos;
}

void MyObject::rotateX(float angle)
{
	m_AngleX += angle;
}

void MyObject::rotateY(float angle)
{
	m_AngleY += angle;
}

void MyObject::rotateZ(float angle)
{
	m_AngleZ += angle;
}

void MyObject::setScale(glm::vec3 scale)
{
	m_Scale = scale;
}

glm::mat4 MyObject::getModelMatrix()
{
	//unity:缩放，旋转，平移
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, m_Scale);//缩放
	model = glm::rotate(model, glm::radians(m_AngleX), glm::vec3(1.0f, 0.0f, 0.0f));//pitch 
	model = glm::rotate(model, glm::radians(m_AngleY), glm::vec3(0.0f, 1.0f, 0.0f));//yaw 
	model = glm::rotate(model, glm::radians(m_AngleZ), glm::vec3(0.0f, 0.0f, 1.0f));//roll


	model = glm::translate(glm::mat4(1.0f), m_Position) * model;//平移

	return model;
}
