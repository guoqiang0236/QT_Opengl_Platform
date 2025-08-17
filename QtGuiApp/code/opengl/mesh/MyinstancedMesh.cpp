#include "MyinstancedMesh.h"
namespace MyOpenGL {
	MyInstancedMesh::MyInstancedMesh(
		MyGeometry* geometry,
		MyMaterial* material,
		unsigned int instanceCount) :
		MyMesh(geometry, material) {  //���ø�����вι��캯��

		initializeOpenGLFunctions();

		m_Type = ObjectType::InstancedMesh;
		mInstanceCount = instanceCount;
		mInstanceMatrices.resize(instanceCount);//���ʱ�����Ǹ������飬��ռλ������ᵥ��������
		
		//vbo
		glGenBuffers(1, &mMatrixVbo); //����һ������������
		glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mInstanceCount, mInstanceMatrices.data(), GL_DYNAMIC_DRAW);

		//vao
		glBindVertexArray(mGeometry->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
		for (int i = 0; i < 4; i++)
		{
			glEnableVertexAttribArray(4 + i);
			glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * i * 4));
			glVertexAttribDivisor(4 + i, 1);//��ʵ������
		}
		glBindVertexArray(0);

	}


	MyInstancedMesh::~MyInstancedMesh() {
	}
	void MyInstancedMesh::sortMatrices(glm::mat4 viewMatrix)
	{
		std::sort(
			mInstanceMatrices.begin(),
			mInstanceMatrices.end(),
			[viewMatrix](const glm::mat4& a, const glm::mat4& b) {
				//1 ����A�����ϵ��Z
				auto modelMatrixA = a;
				auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);//���е���Ƭ�ʼ������ԭ���
				//��������ģ�ͱ任�����������ռ������
				auto cameraPositionA = viewMatrix * worldPositionA;

				//2 ����B�����ϵ��Z
				auto modelMatrixB = b;
				auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);//���е���Ƭ�ʼ������ԭ���
				auto cameraPositionB = viewMatrix * worldPositionB;

				return cameraPositionA.z < cameraPositionB.z;
			});
	}
	void MyInstancedMesh::updateMatrices()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
		//���ʹ��glBufferData�������ݸ��£��ᵼ�����·����Դ�ռ�
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mInstanceCount, mInstanceMatrices, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * mInstanceCount, mInstanceMatrices.data());//�������ר����������VBO���ݵĺ���
		glBindBuffer(GL_ARRAY_BUFFER, 0); //��󻺳���
	}
}