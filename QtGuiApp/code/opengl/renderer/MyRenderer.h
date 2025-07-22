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


class MyRenderer :protected QOpenGLFunctions_4_5_Core {
public:
	MyRenderer();
	~MyRenderer();
	//渲染功能函数:
	//1 每次调用都会渲染一帧
	void render(const std::vector<MyMesh*>& meshes,
		MyCamera* camera,
		MyDirectionalLight* dirLight,
		MyAmbientLight* ambLight
	);
	void render(const std::vector<MyMesh*>& meshes,
		MyCamera* camera,
		MyPointLight* pointLight,
		MyAmbientLight* ambLight
	);
	void render(MyScene * scene,
		MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
		MySpotLight* spotLight, MyAmbientLight* ambLight,bool bshow =true);

	void rendererObject(
		MyObject* object,
		MyCamera* camera, MyDirectionalLight* dirLight, std::vector<MyPointLight*>& pointLights,
		MySpotLight* spotLight, MyAmbientLight* ambLight, bool bshow = true 
	);
	
private:
	//根据Material类型不同,挑选不同的shader
	MyShader* pickShader(MaterialType type);
private:
	//生成多种不同的shader对象
	MyShader* mPhongShader{ nullptr };
	MyShader* mWhiteShader{ nullptr };
	MyShader* mImageShader{ nullptr };
};