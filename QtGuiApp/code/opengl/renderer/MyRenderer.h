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
			OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
			MySpotLight* spotLight, MyAmbientLight* ambLight, bool bshow = true);

		void rendererObject(
			MyOpenGL::MyObject* object,
			OpenGLCamera::MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
			MySpotLight* spotLight, MyAmbientLight* ambLight, bool bshow = true
		);

	private:
		//根据Material类型不同,挑选不同的shader
		MyOpenGL::MyShader* pickShader(MaterialType type);
		void setDepthState(MyOpenGL::MyMaterial* material);
		void setPolygonOffsetState(MyOpenGL::MyMaterial* material);
	private:
		//生成多种不同的shader对象
		MyOpenGL::MyShader* mPhongShader{ nullptr };
		MyOpenGL::MyShader* mWhiteShader{ nullptr };
		MyOpenGL::MyShader* mImageShader{ nullptr };
		MyOpenGL::MyShader* mDepthShader{ nullptr };
	};
}