#include "MyCamera.h"

MyCamera::MyCamera(QObject* parent)
{
}

MyCamera::~MyCamera()
{
}

glm::mat4 MyCamera::getProjectionMatrix()
{
  
	//相机基类返回单位矩阵
	return glm::identity<glm::mat4>();
}
