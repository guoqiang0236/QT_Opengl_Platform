#pragma once

#include "../../pch.h"
#include "../MyMesh.h"
#include "../../camera/MyCamera.h"
#include "../Light/MyDirectionalLight.h"
#include "../Light/MyAmbientLight.h"
#include "../MyShader.h"


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

private:
	//根据Material类型不同,挑选不同的shader
	MyShader* pickShader(MaterialType type);
private:
	//生成多种不同的shader对象
	MyShader* mPhongShader{ nullptr };
	MyShader* mWhiteShader{ nullptr };
};