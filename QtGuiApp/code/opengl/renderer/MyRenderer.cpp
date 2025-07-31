#include "MyRenderer.h"
#include "../Material/MyPhongMaterial.h"
#include "../Material/MyWhiteMaterial.h"
#include "../Material/MyImageMaterial.h"
namespace MyOpenGL {
	MyRenderer::MyRenderer()
	{
		initializeOpenGLFunctions();
		mPhongShader = new MyOpenGL::MyShader("../assets/shaders/phong.vert", "../assets/shaders/phong.frag");
		mWhiteShader = new MyOpenGL::MyShader("../assets/shaders/white.vert", "../assets/shaders/white.frag");
		mImageShader = new MyOpenGL::MyShader("../assets/shaders/image.vert", "../assets/shaders/image.frag");
		mDepthShader = new MyOpenGL::MyShader("../assets/shaders/depth.vert", "../assets/shaders/depth.frag");
	}

	MyRenderer::~MyRenderer()
	{
		if (mPhongShader)
		{
			delete mPhongShader;
			mPhongShader = nullptr;
		}
	}

	void MyRenderer::render(const std::vector<MyOpenGL::MyMesh*>& meshes,
		OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, MyAmbientLight* ambLight)
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
				phongMat->mSpecularMask->bind();
				//mvp����ĸ���
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

				auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
				shader->setMatrix3x3("normalMatrix", normalMatrix);

				// ��Դ������uniform����
				//directionalLight
				shader->setVector3("MydirectionalLight.direction", dirLight->mDirection);
				shader->setVector3("MydirectionalLight.color", dirLight->mColor);
				shader->setFloat("MydirectionalLight.specularIntensity", dirLight->mSpecularIntensity);
				shader->setFloat("MydirectionalLight.intensity", dirLight->mIntensity);
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

	void MyRenderer::render(const std::vector<MyOpenGL::MyMesh*>& meshes,
		OpenGLCamera::MyCamera* camera, MyPointLight* pointLight, MyAmbientLight* ambLight)
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

	void MyRenderer::render(MyOpenGL::MyScene* scene,
		OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
		MySpotLight* spotLight, MyAmbientLight* ambLight, bool bshow)
	{
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

		//����ɫ���
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//2 ������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (!bshow)
			return;
		//3 ��scene�������ڵ㿪ʼ�ݹ���Ⱦ
		rendererObject(scene, camera, dirLight, pointLights, spotLight, ambLight, bshow);
	}

	//��Ե���object
	void MyRenderer::rendererObject(MyOpenGL::MyObject* object, OpenGLCamera::MyCamera* camera,
		MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
		MySpotLight* spotLight, MyAmbientLight* ambLight, bool bshow)
	{
		//1 �ж���Mesh����Object�������Mesh����Ҫ��Ⱦ
		if (object->getType() == MyOpenGL::ObjectType::Mesh) {
			auto mesh = (MyOpenGL::MyMesh*)object;
			auto geometry = mesh->mGeometry;
			auto material = mesh->mMaterial;

			//������Ⱦ״̬
			setDepthState(material);
			setPolygonOffsetState(material);
			setStencilState(material);
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
			default:
				break;
			}

			// 3. ��vao
			glBindVertexArray(geometry->getVao());

			// 4. ִ�л�������
			glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0));
		}

		//2 ����Object���ӽڵ㣬��ÿ���ӽڵ㶼��Ҫ����rendererObject
		auto children = object->getChildren();
		for (int i = 0;i < children.size();i++)
		{
			
			rendererObject(children[i], camera, dirLight, pointLights, spotLight, ambLight, bshow);
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
}

