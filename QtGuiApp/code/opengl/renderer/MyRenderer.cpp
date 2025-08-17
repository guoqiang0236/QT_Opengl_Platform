#include "MyRenderer.h"
#include "../Material/MyPhongMaterial.h"
#include "../Material/MyWhiteMaterial.h"
#include "../Material/MyImageMaterial.h"
#include "../Material/MyopacityMaskMaterial.h"
#include "../Material/MyscreenMaterial.h"
#include "../Material/MycubeMaterial.h"
#include "../Material/MyphongEnvMaterial.h"
#include "../Material/MyphongInstanceMaterial.h"
#include "../Material/MygrassInstanceMaterial.h"
#include "../Mesh/MyinstancedMesh.h"
namespace MyOpenGL {
	MyRenderer::MyRenderer()
	{
		initializeOpenGLFunctions();
		mPhongShader = new MyOpenGL::MyShader("../assets/shaders/phong.vert", "../assets/shaders/phong.frag");
		mWhiteShader = new MyOpenGL::MyShader("../assets/shaders/white.vert", "../assets/shaders/white.frag");
		mImageShader = new MyOpenGL::MyShader("../assets/shaders/image.vert", "../assets/shaders/image.frag");
		mDepthShader = new MyOpenGL::MyShader("../assets/shaders/depth.vert", "../assets/shaders/depth.frag");
		mOpacityMaskShader = new MyOpenGL::MyShader("../assets/shaders/phongOpacityMask.vert", "../assets/shaders/phongOpacityMask.frag");
		mScreenShader = new MyOpenGL::MyShader("../assets/shaders/screen.vert", "../assets/shaders/screen.frag");
		mCubeShader = new MyOpenGL::MyShader("../assets/shaders/cube.vert", "../assets/shaders/cube.frag");
		mPhongEnvShader = new MyOpenGL::MyShader("../assets/shaders/phongEnv.vert", "../assets/shaders/PhongEnv.frag");
		mPhongInstancedShader = new MyOpenGL::MyShader("../assets/shaders/phongInstance.vert", "../assets/shaders/phongInstance.frag");
		mGrassInstanceShader = new MyOpenGL::MyShader("../assets/shaders/grassInstance.vert", "../assets/shaders/grassInstance.frag");
		m_timer.start();
	}

	MyRenderer::~MyRenderer()
	{
		if (mPhongShader)
		{
			delete mPhongShader;
			mPhongShader = nullptr;
		}
	}



	void MyRenderer::render(MyOpenGL::MyScene* scene,
		OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
		MySpotLight* spotLight, MyAmbientLight* ambLight, unsigned int fbo)
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, fbo);// ��֡�������

		//1 ���õ�ǰ֡���Ƶ�ʱ��,opengl�ı�Ҫ״̬������
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);//��֤ÿһ֡��˳�������Ȼ���(����������һ��mesh�����д�����,��û��clear,����bug)

		glDisable(GL_POLYGON_OFFSET_FILL);// �رն����ƫ�����
		glDisable(GL_POLYGON_OFFSET_LINE);// �رն����ƫ����
 
		//�������� ���û���д��״̬ ��ģ�����д��
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);// fail/zfail/zpass �Ĵ���
		glStencilMask(0xff);//����д�� ��֤��ģ�滺����Ա�����

		//Ĭ����ɫ���
		glDisable(GL_BLEND);

		//2 ������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//�����������
		mOpacityObjects.clear();
		mTransparentObjects.clear();
		projectObject(scene);

		std::sort(mTransparentObjects.begin(), mTransparentObjects.end(),
			[camera](MyOpenGL::MyMesh* a, MyOpenGL::MyMesh* b) {
				auto viewMatrix = camera->getViewMatrix();
				//1 ����A�����ϵ��Z
				auto modelMatrixA = a->getModelMatrix();
				auto worldPositionA = modelMatrixA * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				//��������ģ�ͱ任�����������ռ������
				auto cameraPositionA = viewMatrix * worldPositionA;

				//2 ����B�����ϵ��Z
				auto modelMatrixB = b->getModelMatrix();
				auto worldPositionB= modelMatrixB * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				auto cameraPositionB = viewMatrix * worldPositionB;

				return cameraPositionA.z < cameraPositionB.z; // ZֵԽС��ԽԶ�����
			});

		//3 ��Ⱦ��������
	    //����Ⱦ��͸��
		for (int i = 0; i < mOpacityObjects.size(); i++)
		{
			rendererObject(mOpacityObjects[i], camera, dirLight, pointLights, spotLight, ambLight);
		}

		//����Ⱦ͸��
		for (int i = 0; i < mTransparentObjects.size(); i++)
		{
			rendererObject(mTransparentObjects[i], camera, dirLight, pointLights, spotLight, ambLight);

		}
	}

	//��Ե���object
	void MyRenderer::rendererObject(MyOpenGL::MyObject* object, OpenGLCamera::MyCamera* camera,
		MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
		MySpotLight* spotLight, MyAmbientLight* ambLight)
	{
		//1 �ж���Mesh����Object�������Mesh����Ҫ��Ⱦ
		if (object->getType() == MyOpenGL::ObjectType::Mesh|| object->getType()== MyOpenGL::ObjectType::InstancedMesh) {
			auto mesh = (MyOpenGL::MyMesh*)object;
			if (mesh->getShow() == false)
				return;
			auto geometry = mesh->mGeometry;

			//�����ȫ�ֲ���,��ʹ��ȫ�ֲ���
			MyMaterial* material = nullptr;
			if (mGlobalMaterial != nullptr)
			{
				material = mGlobalMaterial;
			}
			else
			{
				material = mesh->mMaterial;
			}
			
			//������Ⱦ״̬
			setDepthState(material);
			setPolygonOffsetState(material);
			setStencilState(material);
			setBlendState(material);
			setFaceCullingState(material);

			// 1. ����ʹ���ĸ�Shader
			MyOpenGL::MyShader* shader = pickShader(material->mType);


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
				if(phongMat->mSpecularMask)
					phongMat->mSpecularMask->bind();
				//mvp����ĸ���
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

				auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
				shader->setMatrix3x3("normalMatrix", normalMatrix);



				// ��Դ������uniform���� 
				//SpotLight
				if (spotLight)
				{
					shader->setVector3("MySpotLight.position", spotLight->getPosition());
					shader->setVector3("MySpotLight.color", spotLight->mColor);
					shader->setVector3("MySpotLight.targetDirection", spotLight->mTargetDirection);
					shader->setFloat("MySpotLight.specularIntensity", spotLight->mSpecularIntensity);
					shader->setFloat("MySpotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
					shader->setFloat("MySpotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
				}
				
				//directionalLight
				shader->setVector3("MydirectionalLight.color", dirLight->mColor);
				shader->setVector3("MydirectionalLight.direction", dirLight->mDirection);
				shader->setFloat("MydirectionalLight.specularIntensity", dirLight->mSpecularIntensity);
				shader->setFloat("MydirectionalLight.intensity", dirLight->mIntensity);

				//pointLight
				for (int i = 0; i < pointLights.size(); i++) {
					auto pointLight = pointLights[i];

					// ��̬������������
					std::string baseName = "MyPointLight[";
					baseName.append(std::to_string(i));
					baseName.append("]");
					shader->setVector3(baseName + ".color", pointLight->mColor);
					shader->setVector3(baseName + ".position", pointLight->getPosition());
					shader->setFloat(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
					shader->setFloat(baseName + ".k2", pointLight->mK2);  // ������˥��ϵ��
					shader->setFloat(baseName + ".k1", pointLight->mK1);  // ����˥��ϵ��
					shader->setFloat(baseName + ".kc", pointLight->mKc);  // ������˥��ϵ��

				}


				shader->setVector3("ambientColor", ambLight->mColor);
				shader->setFloat("shiness", phongMat->mShiness);

				//�����Ϣ����
				shader->setVector3("cameraPosition", camera->mPosition);

				//͸����
				shader->setFloat("opacity", material->mOpacity);
			}
											break;
			case MaterialType::WhiteMaterial: {
				//mvp����ĸ���
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			}
											break;
			
			case MaterialType::ImageMaterial: {
				//mvp����ĸ���
				shader->setInt("imagesampler", 2);
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			}
											break;
			case MaterialType::DepthMaterial: {
				//mvp����ĸ���
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

				shader->setFloat("near", camera->mNear);
				shader->setFloat("far", camera->mFar);

			}
											break;
			case MaterialType::OpacityMaskMaterial: {
				MyOpacityMaskMaterial* opacityMat = static_cast<MyOpacityMaskMaterial*>(material);
				//�����������������Ԫ�ҹ�
				//diffuse�ɰ��֡����
				shader->setInt("sampler", 0);
				//������������Ԫ���йҹ�
				opacityMat->mDiffuse->bind();
				//opacityMask�ɰ��֡����
				shader->setInt("opacityMaskSampler", 1);
				opacityMat->mOpacityMask->bind();
				//MVP
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

				auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
				shader->setMatrix3x3("normalMatrix", normalMatrix);

				//��Դ������uniform���� 
				//directionalLight�ĸ���
				shader->setVector3("directionalLight.color", dirLight->mColor);
				shader->setVector3("directionalLight.direction", dirLight->mDirection);
				shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

				shader->setFloat("shiness", opacityMat->mShiness);
				//�����Ϣ����
				shader->setVector3("cameraPosition", camera->mPosition);

				//͸����
				shader->setFloat("opacity", material->mOpacity);
			}
											break;
			case MaterialType::ScreenMaterial: {
				MyScreenMaterial* screenMat = static_cast<MyScreenMaterial*>(material);
				shader->setInt("screenTexSampler", 0);
				screenMat->mScreenTexture->bind();
				shader->setFloat("texWidth", m_width);//��д�� �������Ż�
				shader->setFloat("texHeight", m_height);
			}
											 break;
			case MaterialType::CubeMaterial: {
			
				MyCubeMaterial* cubeMat = static_cast<MyCubeMaterial*>(material);

				mesh->setPosition(camera->mPosition);
				//sphericalSampler
				shader->setInt("cubeSampler", 0);
				cubeMat->mDiffuse->bind();
				//MVP
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
				

			
			}
										   break;
			case MaterialType::PhongEnvMaterial: {
				MyPhongEnvMaterial* phongMat = static_cast<MyPhongEnvMaterial*>(material);

				shader->setInt("sampler", 0);
				phongMat->mDiffuse->bind();

				shader->setInt("envSampler", 1);
				phongMat->mEnv->bind();

				shader->setInt("specularMaskSampler", 1);
				if (phongMat->mSpecularMask)
					phongMat->mSpecularMask->bind();
				//mvp����ĸ���
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

				auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
				shader->setMatrix3x3("normalMatrix", normalMatrix);



				// ��Դ������uniform���� 
				//SpotLight
				if (spotLight)
				{
					shader->setVector3("SpotLight.position", spotLight->getPosition());
					shader->setVector3("SpotLight.color", spotLight->mColor);
					shader->setVector3("SpotLight.targetDirection", spotLight->mTargetDirection);
					shader->setFloat("SpotLight.specularIntensity", spotLight->mSpecularIntensity);
					shader->setFloat("SpotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
					shader->setFloat("SpotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
				}

				//directionalLight
				shader->setVector3("directionalLight.color", dirLight->mColor);
				shader->setVector3("directionalLight.direction", dirLight->mDirection);
				shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
				shader->setFloat("directionalLight.intensity", dirLight->mIntensity);

				//pointLight
				for (int i = 0; i < pointLights.size(); i++) {
					auto pointLight = pointLights[i];

					// ��̬������������
					std::string baseName = "PointLight[";
					baseName.append(std::to_string(i));
					baseName.append("]");
					shader->setVector3(baseName + ".color", pointLight->mColor);
					shader->setVector3(baseName + ".position", pointLight->getPosition());
					shader->setFloat(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
					shader->setFloat(baseName + ".k2", pointLight->mK2);  // ������˥��ϵ��
					shader->setFloat(baseName + ".k1", pointLight->mK1);  // ����˥��ϵ��
					shader->setFloat(baseName + ".kc", pointLight->mKc);  // ������˥��ϵ��

				}


				shader->setVector3("ambientColor", ambLight->mColor);
				shader->setFloat("shiness", phongMat->mShiness);

				//�����Ϣ����
				shader->setVector3("cameraPosition", camera->mPosition);

				//͸����
				shader->setFloat("opacity", material->mOpacity);
			}
											break;
			case MaterialType::PhongInstanceMaterial: {
				MyPhongInstanceMaterial* phongMat = static_cast<MyPhongInstanceMaterial*>(material);
				MyInstancedMesh* im = static_cast<MyInstancedMesh*>(mesh);
				//diffuse��ͼ
				//�����������������Ԫ�ҹ�
				shader->setInt("sampler", 0);
				//������������Ԫ���йҹ�
				phongMat->mDiffuse->bind();

			
				//mvp����ĸ���
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

				auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
				shader->setMatrix3x3("normalMatrix", normalMatrix);



				// ��Դ������uniform���� 
				//SpotLight
				if (spotLight)
				{
					shader->setVector3("SpotLight.position", spotLight->getPosition());
					shader->setVector3("SpotLight.color", spotLight->mColor);
					shader->setVector3("SpotLight.targetDirection", spotLight->mTargetDirection);
					shader->setFloat("SpotLight.specularIntensity", spotLight->mSpecularIntensity);
					shader->setFloat("SpotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
					shader->setFloat("SpotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
				}

				//directionalLight
				shader->setVector3("directionalLight.color", dirLight->mColor);
				shader->setVector3("directionalLight.direction", dirLight->mDirection);
				shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
				shader->setFloat("directionalLight.intensity", dirLight->mIntensity);

				//pointLight
				for (int i = 0; i < pointLights.size(); i++) {
					auto pointLight = pointLights[i];

					// ��̬������������
					std::string baseName = "PointLight[";
					baseName.append(std::to_string(i));
					baseName.append("]");
					shader->setVector3(baseName + ".color", pointLight->mColor);
					shader->setVector3(baseName + ".position", pointLight->getPosition());
					shader->setFloat(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
					shader->setFloat(baseName + ".k2", pointLight->mK2);  // ������˥��ϵ��
					shader->setFloat(baseName + ".k1", pointLight->mK1);  // ����˥��ϵ��
					shader->setFloat(baseName + ".kc", pointLight->mKc);  // ������˥��ϵ��

				}


				shader->setVector3("ambientColor", ambLight->mColor);
				shader->setFloat("shiness", phongMat->mShiness);

				//�����Ϣ����
				shader->setVector3("cameraPosition", camera->mPosition);

				//͸����
				shader->setFloat("opacity", material->mOpacity);

				//**********����uniform���͵ľ���任����************
			    //shader->setMatrix4x4Array("matrices", im->mInstanceMatrices, im->mInstanceCount);
			}
											break;
			case MaterialType::GrassInstanceMaterial: {
				MyGrassInstanceMaterial* grassMat = static_cast<MyGrassInstanceMaterial*>(material);
				MyInstancedMesh* im = static_cast<MyInstancedMesh*>(mesh);
				
				im->sortMatrices(camera->getViewMatrix());//��ʵ�����ľ����������
				im->updateMatrices();

				shader->setInt("sampler", 0);
				grassMat->mDiffuse->bind();

				shader->setInt("opacityMask", 1);
				grassMat->mOpacityMask->bind();

				//mvp����ĸ���
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

				/*auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
				shader->setMatrix3x3("normalMatrix", normalMatrix);*/



				// ��Դ������uniform���� 
				//SpotLight
				if (spotLight)
				{
					shader->setVector3("SpotLight.position", spotLight->getPosition());
					shader->setVector3("SpotLight.color", spotLight->mColor);
					shader->setVector3("SpotLight.targetDirection", spotLight->mTargetDirection);
					shader->setFloat("SpotLight.specularIntensity", spotLight->mSpecularIntensity);
					shader->setFloat("SpotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
					shader->setFloat("SpotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
				}

				//directionalLight
				shader->setVector3("directionalLight.color", dirLight->mColor);
				shader->setVector3("directionalLight.direction", dirLight->mDirection);
				shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
				shader->setFloat("directionalLight.intensity", dirLight->mIntensity);

				//pointLight
				for (int i = 0; i < pointLights.size(); i++) {
					auto pointLight = pointLights[i];

					// ��̬������������
					std::string baseName = "PointLight[";
					baseName.append(std::to_string(i));
					baseName.append("]");
					shader->setVector3(baseName + ".color", pointLight->mColor);
					shader->setVector3(baseName + ".position", pointLight->getPosition());
					shader->setFloat(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
					shader->setFloat(baseName + ".k2", pointLight->mK2);  // ������˥��ϵ��
					shader->setFloat(baseName + ".k1", pointLight->mK1);  // ����˥��ϵ��
					shader->setFloat(baseName + ".kc", pointLight->mKc);  // ������˥��ϵ��

				}


				shader->setVector3("ambientColor", ambLight->mColor);
				shader->setFloat("shiness", grassMat->mShiness);

				//�����Ϣ����
				shader->setVector3("cameraPosition", camera->mPosition);

				//͸����
				shader->setFloat("opacity", material->mOpacity);

				//��ͼ����
				shader->setFloat("uvScale", grassMat->mUVScale);
				shader->setFloat("brightness", grassMat->mBrightness);
			
			

				//����
				float time = m_timer.elapsed() / 1000.0f; // ��λ����
				shader->setFloat("time", time);
				shader->setFloat("windScale", grassMat->mWindScale);
				shader->setVector3("windDirection", grassMat->mWindDirection);
				shader->setFloat("phaseScale", grassMat->mPhaseScale);
				//**********����uniform���͵ľ���任����************
				//shader->setMatrix4x4Array("matrices", im->mInstanceMatrices, im->mInstanceCount);
			}
													break;
			default:
				break;
			}

			// 3. ��vao
			glBindVertexArray(geometry->getVao());

			// 4. ִ�л�������
			if(object->getType() == MyOpenGL::ObjectType::InstancedMesh)
			{
				//�����ʵ������mesh,����Ҫ����ʵ����������
				auto im = static_cast<MyInstancedMesh*>(object);
				glDrawElementsInstanced(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0), im->mInstanceCount);
			}
			else {
				glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0));

			}
		}

		//2 ����Object���ӽڵ㣬��ÿ���ӽڵ㶼��Ҫ����rendererObject
		auto children = object->getChildren();
		for (int i = 0;i < children.size();i++)
		{
			
			rendererObject(children[i], camera, dirLight, pointLights, spotLight, ambLight);
		}
	}

	void MyRenderer::projectObject(MyObject* obj)
	{
		if (obj->getType() == MyOpenGL::ObjectType::Mesh|| obj->getType() == MyOpenGL::ObjectType::InstancedMesh)
		{
			MyMesh* mesh = static_cast<MyMesh*>(obj);

			auto material = mesh->mMaterial;

			if (material->mBlend)
			{
				mTransparentObjects.push_back(mesh);
			}
			else
			{
				mOpacityObjects.push_back(mesh);
			}
			
		}

		auto children = obj->getChildren();
		for (int i = 0; i < children.size(); i++)
		{
			projectObject(children[i]);
		}
		
	}

	MyOpenGL::MyShader* MyRenderer::pickShader(MaterialType type)
	{
		MyOpenGL::MyShader* result = nullptr;
		switch (type)
		{
		case MaterialType::PhongMaterial:
			result = mPhongShader;
			break;
		case MaterialType::WhiteMaterial:
			result = mWhiteShader;
			break;
		case MaterialType::ImageMaterial:
			result = mImageShader;
			break;
		case MaterialType::DepthMaterial:
			result = mDepthShader;
			break;
		case MaterialType::OpacityMaskMaterial:
			result = mOpacityMaskShader;
			break;
		case MaterialType::ScreenMaterial:
			result = mScreenShader;
			break;
		case MaterialType::CubeMaterial:
			result = mCubeShader;
			break;
		case MaterialType::PhongEnvMaterial:
			result = mPhongEnvShader;
			break;
		case MaterialType::PhongInstanceMaterial:
			result = mPhongInstancedShader;
			break;
		case MaterialType::GrassInstanceMaterial:
			result = mGrassInstanceShader;
			break;
		default:
			std::cout << "Unknown material type to pick shader" << std::endl;
			break;
		}
		return result;
	}


	

	void MyRenderer::setDepthState(MyOpenGL::MyMaterial* material)
	{
		//������״̬
		if (material->mDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(material->mDepthFunc);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		if (material->mDepthWrite)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}
	}
	void MyRenderer::setPolygonOffsetState(MyOpenGL::MyMaterial* material)
	{
		// �������ƫ��״̬
		if (material->mPolygonOffset)
		{
			glEnable(material->mPolygonOffsetType);
			glPolygonOffset(material->mFactor, material->mUnit);
		}
		else
		{
			glDisable(material->mPolygonOffsetType);
		}
	}

	void MyRenderer::setStencilState(MyOpenGL::MyMaterial* material) {
		if (material->mStencilTest)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilOp(material->mSFail, material->mZFail, material->mZPass);
			glStencilMask(material->mStencilMask);
			glStencilFunc(material->mStencilFunc, material->mStencilRef, material->mStencilFuncMask);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}
	void MyRenderer::setBlendState(MyOpenGL::MyMaterial* material)
	{
		if (material->mBlend)
		{
			glEnable(GL_BLEND);
			glBlendFunc(material->mSFactor, material->mDFactor);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}
	void MyRenderer::setFaceCullingState(MyOpenGL::MyMaterial* material)
	{
		if (material->mFaceCulling)
		{
			glEnable(GL_CULL_FACE);
			glFrontFace(material->mFrontFace);
			glCullFace(material->mCullFace);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
}

