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
	//1 设置当前帧绘制的时候,opengl的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//3 遍历mesh渲染
    for (int i = 0; i < meshes.size(); i++) {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 1. 决定使用哪个Shader
		MyShader* shader = pickShader(material->mType);


        // 2. 更新shader的uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			MyPhongMaterial* phongMat = static_cast<MyPhongMaterial*>(material);
			
			//diffuse贴图
		    //将纹理采样器与纹理单元挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->mDiffuse->bind();

			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();
			//mvp矩阵的更新
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			// 光源参数的uniform更新
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
			shader->setFloat("directionalLight.intensity", dirLight->mIntensity);
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("shiness", phongMat->mShiness);

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);
		}
			break;
		case MaterialType::WhiteMaterial: {
			//mvp矩阵的更新
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

		}
			break;
		default:
			continue;
		}
		
        // 3. 绑定vao
		glBindVertexArray(geometry->getVao());

        // 4. 执行绘制命令
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0));
		
    }
}

void MyRenderer::render(const std::vector<MyMesh*>& meshes, MyCamera* camera, MyPointLight* pointLight, MyAmbientLight* ambLight)
{
	//1 设置当前帧绘制的时候,opengl的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//3 遍历mesh渲染
	for (int i = 0; i < meshes.size(); i++) {
		auto mesh = meshes[i];
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;

		// 1. 决定使用哪个Shader
		MyShader* shader = pickShader(material->mType);


		// 2. 更新shader的uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			MyPhongMaterial* phongMat = static_cast<MyPhongMaterial*>(material);

			//diffuse贴图
			//将纹理采样器与纹理单元挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->mDiffuse->bind();

			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();
			//mvp矩阵的更新
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			// 光源参数的uniform更新
			//光源参数的uniform更新
			shader->setVector3("MyPointLight.position", pointLight->getPosition());
			shader->setVector3("MyPointLight.color", pointLight->mColor);
			shader->setFloat("MyPointLight.specularIntensity", pointLight->mSpecularIntensity);
			shader->setFloat("MyPointLight.k2", pointLight->mK2);  // 二次项衰减系数
			shader->setFloat("MyPointLight.k1", pointLight->mK1);  // 线性衰减系数
			shader->setFloat("MyPointLight.kc", pointLight->mKc);  // 常数项衰减系数
	 
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("shiness", phongMat->mShiness);

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);
		}
										break;
		case MaterialType::WhiteMaterial: {
			//mvp矩阵的更新
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

		}
										break;
		default:
			continue;
		}

		// 3. 绑定vao
		glBindVertexArray(geometry->getVao());

		// 4. 执行绘制命令
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, (void*)(sizeof(int) * 0));

	} 
}

void MyRenderer::render(const std::vector<MyMesh*>& meshes, MyCamera* camera, MySpotLight* spotLight, MyAmbientLight* ambLight)
{
	//1 设置当前帧绘制的时候,opengl的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//3 遍历mesh渲染
	for (int i = 0; i < meshes.size(); i++) {
		auto mesh = meshes[i];
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;

		// 1. 决定使用哪个Shader
		MyShader* shader = pickShader(material->mType);


		// 2. 更新shader的uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			MyPhongMaterial* phongMat = static_cast<MyPhongMaterial*>(material);

			//diffuse贴图
			//将纹理采样器与纹理单元挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->mDiffuse->bind();

			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();
			//mvp矩阵的更新
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);



			// 光源参数的uniform更新 
			shader->setVector3("MySpotLight.position", spotLight->getPosition());
			shader->setVector3("MySpotLight.color", spotLight->mColor);
			shader->setVector3("MySpotLight.targetDirection", spotLight->mTargetDirection);
			shader->setFloat("MySpotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setFloat("MySpotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
			shader->setFloat("MySpotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));

			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("shiness", phongMat->mShiness);

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);
		}
										break;
		case MaterialType::WhiteMaterial: {
			//mvp矩阵的更新
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

		}
										break;
		default:
			continue;
		}

		// 3. 绑定vao
		glBindVertexArray(geometry->getVao());

		// 4. 执行绘制命令
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


