#include "MyCamera.h"

MyCamera::MyCamera()
{
}

MyCamera::~MyCamera()
{
}

glm::mat4 MyCamera::getViewMatrix()
{
    // lookAt
    // - eye:    相机位置（使用 mPosition）
    // - center: 看向世界坐标的哪个点
    // - top:    穹顶（使用 mUp 替代）
	glm::vec3 front = glm::cross(mUp, mRight);
	glm::vec3 center = mPosition + front;

	return glm::lookAt(mPosition, center, mUp);
}
