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
		//渲染功能函数:
		//1 每次调用都会渲染一帧
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
		MyMaterial* mGlobalMaterial{ nullptr }; //全局材质

	private:

		void projectObject(MyObject* obj);

		//根据Material类型不同,挑选不同的shader
		MyOpenGL::MyShader* pickShader(MaterialType type);
		void setDepthState(MyOpenGL::MyMaterial* material);
		void setPolygonOffsetState(MyOpenGL::MyMaterial* material);
		void setStencilState(MyOpenGL::MyMaterial* material);
		void setBlendState(MyOpenGL::MyMaterial* material);
		void setFaceCullingState(MyOpenGL::MyMaterial* material);
	private:
		//生成多种不同的shader对象
		MyOpenGL::MyShader* mPhongShader{ nullptr };
		MyOpenGL::MyShader* mWhiteShader{ nullptr };
		MyOpenGL::MyShader* mImageShader{ nullptr };
		MyOpenGL::MyShader* mDepthShader{ nullptr };
		MyOpenGL::MyShader* mOpacityMaskShader{ nullptr };
		MyOpenGL::MyShader* mScreenShader{ nullptr };


		//不透明物体与透明物体的队列
		//注意!! 每一帧绘制前需要清空两个队列
		std::vector<MyOpenGL::MyMesh*> mOpacityObjects{};
		std::vector<MyOpenGL::MyMesh*> mTransparentObjects{};
	};
}