#include "MyRenderer.h"
#include "../Material/MyPhongMaterial.h"
#include "../Material/MyWhiteMaterial.h"
MyRenderer::MyRenderer()
{
	initializeOpenGLFunctions();
	mPhongShader = new MyShader("../assets/shaders/phong.vert", "../assets/shaders/phong.frag");
	mWhiteShader = new MyShader("../assets/shaders/white.vert", "../assets/shaders/white.frag");
}

MyRenderer::~MyRenderer()
{
	if (mPhongShader)
	{
		delete mPhongShader;
		mPhongShader = nullptr;
	}
}

void MyRenderer::render(const std::vector<MyMesh*>& meshes, MyCamera* camera, MyDirectionalLight* dirLight, MyAmbientLight* ambLight)
{
	//1 ���õ�ǰ֡���Ƶ�ʱ��,opengl�ı�Ҫ״̬������
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//3 ����mesh��Ⱦ
    for (int i = 0; i < meshes.size(); i++) {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 1. ����ʹ���ĸ�Shader
		MyShader* shader = pickShader(material->mType);


        // 2. ����shader��uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			MyPhongMaterial* phongMat = static_cast<MyPhongMaterial*>(material);
			
			//diffuse��ͼ
		    //�����������������Ԫ�ҹ�
			shader->setInt("sampler", 0);
			//������������Ԫ���йҹ�
			phongMat->mDiffuse->bind();

			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();
			//mvp����ĸ���
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			// ��Դ������uniform����
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
			shader->setFloat("directionalLight.intensity", dirLight->mIntensity);
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("shiness", phongMat->mShiness);

			//�����Ϣ����
			shader->setVector3("cameraPosition", camera->mPosition);
		}
			break;
		case MaterialType::WhiteMaterial: {
			//mvp����ĸ���
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

		}
			break;
		default:
			continue;
		}
		
        // 3. ��vao
		glBindVertexArray(geometry->getVao());

        // 4. ִ�л�������
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0));
		
    }
}

void MyRenderer::render(const std::vector<MyMesh*>& meshes, MyCamera* camera, MyPointLight* pointLight, MyAmbientLight* ambLight)
{
	//1 ���õ�ǰ֡���Ƶ�ʱ��,opengl�ı�Ҫ״̬������
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//3 ����mesh��Ⱦ
	for (int i = 0; i < meshes.size(); i++) {
		auto mesh = meshes[i];
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;

		// 1. ����ʹ���ĸ�Shader
		MyShader* shader = pickShader(material->mType);


		// 2. ����shader��uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			MyPhongMaterial* phongMat = static_cast<MyPhongMaterial*>(material);

			//diffuse��ͼ
			//�����������������Ԫ�ҹ�
			shader->setInt("sampler", 0);
			//������������Ԫ���йҹ�
			phongMat->mDiffuse->bind();

			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();
			//mvp����ĸ���
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			// ��Դ������uniform����
			//��Դ������uniform����
			shader->setVector3("MyPointLight.position", pointLight->getPosition());
			shader->setVector3("MyPointLight.color", pointLight->mColor);
			shader->setFloat("MyPointLight.specularIntensity", pointLight->mSpecularIntensity);
			shader->setFloat("MyPointLight.k2", pointLight->mK2);  // ������˥��ϵ��
			shader->setFloat("MyPointLight.k1", pointLight->mK1);  // ����˥��ϵ��
			shader->setFloat("MyPointLight.kc", pointLight->mKc);  // ������˥��ϵ��
	 
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("shiness", phongMat->mShiness);

			//�����Ϣ����
			shader->setVector3("cameraPosition", camera->mPosition);
		}
										break;
		case MaterialType::WhiteMaterial: {
			//mvp����ĸ���
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

		}
										break;
		default:
			continue;
		}

		// 3. ��vao
		glBindVertexArray(geometry->getVao());

		// 4. ִ�л�������
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0));

	} 
}

void MyRenderer::render(const std::vector<MyMesh*>& meshes, MyCamera* camera, MySpotLight* spotLight, MyAmbientLight* ambLight)
{
	//1 ���õ�ǰ֡���Ƶ�ʱ��,opengl�ı�Ҫ״̬������
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//3 ����mesh��Ⱦ
	for (int i = 0; i < meshes.size(); i++) {
		auto mesh = meshes[i];
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;

		// 1. ����ʹ���ĸ�Shader
		MyShader* shader = pickShader(material->mType);


		// 2. ����shader��uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			MyPhongMaterial* phongMat = static_cast<MyPhongMaterial*>(material);

			//diffuse��ͼ
			//�����������������Ԫ�ҹ�
			shader->setInt("sampler", 0);
			//������������Ԫ���йҹ�
			phongMat->mDiffuse->bind();

			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();
			//mvp����ĸ���
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);



			// ��Դ������uniform���� 
			shader->setVector3("MySpotLight.position", spotLight->getPosition());
			shader->setVector3("MySpotLight.color", spotLight->mColor);
			shader->setVector3("MySpotLight.targetDirection", spotLight->mTargetDirection);
			shader->setFloat("MySpotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setFloat("MySpotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
			shader->setFloat("MySpotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));

			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("shiness", phongMat->mShiness);

			//�����Ϣ����
			shader->setVector3("cameraPosition", camera->mPosition);
		}
										break;
		case MaterialType::WhiteMaterial: {
			//mvp����ĸ���
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

		}
										break;
		default:
			continue;
		}

		// 3. ��vao
		glBindVertexArray(geometry->getVao());

		// 4. ִ�л�������
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0));

	}
}

MyShader* MyRenderer::pickShader(MaterialType type)
{
	MyShader* result = nullptr;
	switch (type)
	{
	case MaterialType::PhongMaterial:
		result = mPhongShader;
		break;
	case MaterialType::WhiteMaterial:
		result = mWhiteShader;
		break;
	//case MaterialType::DepthMaterial:
	//	result = mDepthShader;
	//	break;
	//case MaterialType::OpacityMaskMaterial:
	//	result = mOpacityMaskShader;
	//	break;
	//case MaterialType::ScreenMaterial:
	//	result = mScreenShader;
	//	break;
	//case MaterialType::CubeMaterial:
	//	result = mCubeShader;
	//	break;
	//case MaterialType::PhongEnvMaterial:
	//	result = mPhongEnvShader;
	//	break;
	//case MaterialType::PhongInstanceMaterial:
	//	result = mPhongInstancedShader;
	//	break;
	//case MaterialType::GrassInstanceMaterial:
	//	result = mGrassInstanceShader;
	//	break;
	default:
		std::cout << "Unknown material type to pick shader" << std::endl;
		break;
	}
	return result;
}


