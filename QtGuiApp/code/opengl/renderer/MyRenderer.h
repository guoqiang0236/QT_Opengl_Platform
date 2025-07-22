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
	//��Ⱦ���ܺ���:
	//1 ÿ�ε��ö�����Ⱦһ֡
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
	//����Material���Ͳ�ͬ,��ѡ��ͬ��shader
	MyShader* pickShader(MaterialType type);
private:
	//���ɶ��ֲ�ͬ��shader����
	MyShader* mPhongShader{ nullptr };
	MyShader* mWhiteShader{ nullptr };
	MyShader* mImageShader{ nullptr };
};