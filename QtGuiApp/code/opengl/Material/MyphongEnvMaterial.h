#pragma once
#include "MyMaterial.h"
#include "../MyTexture.h"

namespace MyOpenGL {
	class MyPhongEnvMaterial : public MyMaterial
	{
	public:
		MyPhongEnvMaterial();
		~MyPhongEnvMaterial();

		MyTexture* mDiffuse{ nullptr };
		MyTexture* mSpecularMask{ nullptr };
		float mShiness{ 1.0f };
		MyTexture* mEnv{ nullptr };

	private:

	};
}
