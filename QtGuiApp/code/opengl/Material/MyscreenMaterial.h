#pragma once
#include "MyMaterial.h"
#include "../MyTexture.h"
namespace MyOpenGL {
	class MyScreenMaterial : public MyMaterial
	{
	public:
		MyScreenMaterial();
		~MyScreenMaterial();

		MyTexture* mScreenTexture{ nullptr };

	private:

	};
}
