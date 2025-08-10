#pragma once
#include "MyMaterial.h"
#include "../MyTexture.h"

namespace MyOpenGL {
	class MyPhongInstanceMaterial : public MyMaterial
	{
	public:
		MyPhongInstanceMaterial();
		~MyPhongInstanceMaterial();

		MyTexture* mDiffuse{ nullptr };
		MyTexture* mSpecularMask{ nullptr };
		float mShiness{ 1.0f };

	private:

	};

}