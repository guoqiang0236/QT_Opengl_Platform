#pragma once

#include "MyMaterial.h"
#include "../MyTexture.h"

namespace MyOpenGL {
class MyOpacityMaskMaterial : public MyMaterial
{
public:
	MyOpacityMaskMaterial();
	~MyOpacityMaskMaterial();

	MyOpenGL::MyTexture* mDiffuse{ nullptr };
	MyOpenGL::MyTexture* mOpacityMask{ nullptr };
	float mShiness{ 1.0f };

private:

};
}
