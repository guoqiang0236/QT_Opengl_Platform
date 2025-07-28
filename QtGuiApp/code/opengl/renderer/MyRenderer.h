#pragma once

#include "../../pch.h"
#include "../MyMesh.h"
#include "../../camera/MyCamera.h"
#include "../Light/MyDirectionalLight.h"
#include "../Light/MyPointLight.h"
#include "../Light/MySpotLight.h"
#include "../Light/MyAmbientLight.h"
#include "../MyShader.h"
#include "../MyScene.h"

namespace MyOpenGL {
	class MyRenderer :protected QOpenGLFunctions_4_5_Core {
	public:
		MyRenderer();
		~MyRenderer();
		//��Ⱦ���ܺ���:
		//1 ÿ�ε��ö�����Ⱦһ֡
		void render(const std::vector<MyOpenGL::MyMesh*>& meshes,
			OpenGLCamera::MyCamera* camera,
			MyDirectionalLight* dirLight,
			MyAmbientLight* ambLight
		);
		void render(const std::vector<MyOpenGL::MyMesh*>& meshes,
			OpenGLCamera::MyCamera* camera,
			MyPointLight* pointLight,
			MyAmbientLight* ambLight
		);
		void render(MyOpenGL::MyScene* scene,
			OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
			MySpotLight* spotLight, MyAmbientLight* ambLight, bool bshow = true);

		void rendererObject(
			MyOpenGL::MyObject* object,
			OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
			MySpotLight* spotLight, MyAmbientLight* ambLight, bool bshow = true
		);

	private:
		//����Material���Ͳ�ͬ,��ѡ��ͬ��shader
		MyOpenGL::MyShader* pickShader(MaterialType type);
		void setDepthState(MyOpenGL::MyMaterial* material);
		void setPolygonOffsetState(MyOpenGL::MyMaterial* material);
	private:
		//���ɶ��ֲ�ͬ��shader����
		MyOpenGL::MyShader* mPhongShader{ nullptr };
		MyOpenGL::MyShader* mWhiteShader{ nullptr };
		MyOpenGL::MyShader* mImageShader{ nullptr };
		MyOpenGL::MyShader* mDepthShader{ nullptr };
	};
}