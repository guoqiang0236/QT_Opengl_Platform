#include "MyObject.h"
namespace MyOpenGL {
	MyObject::MyObject()
	{
		m_Type = ObjectType::Object;
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

	void MyObject::setAngleX(float angle)
	{
		m_AngleX = angle;
	}

	void MyObject::setAngleY(float angle)
	{
		m_AngleY = angle;
	}

	void MyObject::setAngleZ(float angle)
	{
		m_AngleZ = angle;
	}

	void MyObject::setScale(glm::vec3 scale)
	{
		m_Scale = scale;
		//qDebug() << "setScale:" << m_Scale.x << m_Scale.y << m_Scale.z;
	}

	glm::mat4 MyObject::getModelMatrix()const
	{
		//���Ȼ�ȡ���׵ı任����
		glm::mat4 parentMatrix{ 1.0f };
		if (m_Parent != nullptr)
		{
			parentMatrix = m_Parent->getModelMatrix();
		}


		//unity:���ţ���ת��ƽ��
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, m_Scale);//����
		model = glm::rotate(model, glm::radians(m_AngleX), glm::vec3(1.0f, 0.0f, 0.0f));//pitch 
		model = glm::rotate(model, glm::radians(m_AngleY), glm::vec3(0.0f, 1.0f, 0.0f));//yaw 
		model = glm::rotate(model, glm::radians(m_AngleZ), glm::vec3(0.0f, 0.0f, 1.0f));//roll


		model = parentMatrix * glm::translate(glm::mat4(1.0f), m_Position) * model;//ƽ��

		return model;
	}

	void MyObject::addChild(MyObject* obj)
	{
		//1. ����Ƿ���������Ӷ���
		auto iter = std::find(m_Children.begin(), m_Children.end(), obj);
		if (iter != m_Children.end())
		{
			return;
		}
		//2. ����Ӷ����Ӷ����б�
		m_Children.push_back(obj);

		//3. ���ø�����ָ��
		obj->m_Parent = this;
	}

	std::vector<MyObject*> MyObject::getChildren()
	{
		return m_Children;
	}

	MyObject* MyObject::getParent()
	{
		return m_Parent;
	}
}