#include "MyCamera.h"
namespace OpenGLCamera {
    MyCamera::MyCamera(QObject* parent)
    {
    }

    MyCamera::~MyCamera()
    {
    }

    glm::mat4 MyCamera::getViewMatrix()
    {
        // lookat������㣺
       // - eye:    ���λ�ã�ʹ��mPosition��
       // - center: ��������������ĸ���
       // - up:     񷶥����ʹ��mUp�����

       // 1. ���������ǰ����������������ϵ��
        glm::vec3 front = glm::cross(mUp, mRight);

        // 2. ȷ���۲�Ŀ��㣨���λ��+ǰ��������
        glm::vec3 center = mPosition + front;

        // 3. ������ͼ����
        return glm::lookAt(mPosition, center, mUp);
    }

    glm::mat4 MyCamera::getProjectionMatrix()
    {

        //������෵�ص�λ����
        return glm::identity<glm::mat4>();
    }

    void MyCamera::scale(float deltaScale)
    {
    }
}