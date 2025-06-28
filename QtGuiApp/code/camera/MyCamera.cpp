#include "MyCamera.h"

MyCamera::MyCamera(QObject* parent)
{
}

MyCamera::~MyCamera()
{
}

glm::mat4 MyCamera::getViewMatrix()
{
    // lookat矩阵计算：
   // - eye:    相机位置（使用mPosition）
   // - center: 看向世界坐标的哪个点
   // - up:     穹顶方向（使用mUp替代）

   // 1. 计算相机正前方向量（右手坐标系）
    glm::vec3 front = glm::cross(mUp, mRight);

    // 2. 确定观察目标点（相机位置+前方向量）
    glm::vec3 center = mPosition + front;

    // 3. 生成视图矩阵
    return glm::lookAt(mPosition, center, mUp);
}

glm::mat4 MyCamera::getProjectionMatrix()
{
  
	//相机基类返回单位矩阵
	return glm::identity<glm::mat4>();
}

void MyCamera::scale(float deltaScale)
{
}
