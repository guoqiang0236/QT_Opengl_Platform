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
			OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, 
			std::vector<MyPointLight*>& pointLights,
			MySpotLight* spotLight, MyAmbientLight* ambLight,
			unsigned int fbo = 0
		);

		void rendererObject(
			MyOpenGL::MyObject* object,
			OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
			MySpotLight* spotLight, MyAmbientLight* ambLight
		);
	public:
		MyMaterial* mGlobalMaterial{ nullptr }; //ȫ�ֲ���

	private:

		void projectObject(MyObject* obj);

		//����Material���Ͳ�ͬ,��ѡ��ͬ��shader
		MyOpenGL::MyShader* pickShader(MaterialType type);
		void setDepthState(MyOpenGL::MyMaterial* material);
		void setPolygonOffsetState(MyOpenGL::MyMaterial* material);
		void setStencilState(MyOpenGL::MyMaterial* material);
		void setBlendState(MyOpenGL::MyMaterial* material);
		void setFaceCullingState(MyOpenGL::MyMaterial* material);
	private:
		//���ɶ��ֲ�ͬ��shader����
		MyOpenGL::MyShader* mPhongShader{ nullptr };
		MyOpenGL::MyShader* mWhiteShader{ nullptr };
		MyOpenGL::MyShader* mImageShader{ nullptr };
		MyOpenGL::MyShader* mDepthShader{ nullptr };
		MyOpenGL::MyShader* mOpacityMaskShader{ nullptr };
		MyOpenGL::MyShader* mScreenShader{ nullptr };


		//��͸��������͸������Ķ���
		//ע��!! ÿһ֡����ǰ��Ҫ�����������
		std::vector<MyOpenGL::MyMesh*> mOpacityObjects{};
		std::vector<MyOpenGL::MyMesh*> mTransparentObjects{};
	};
}