#pragma once
#include "MyMaterial.h"
#include "../MyTexture.h"
namespace MyOpenGL {
	class MyCubeMaterial : public MyMaterial
	{
	public:
		MyCubeMaterial();
		~MyCubeMaterial();

		MyTexture* mDiffuse{ nullptr };

	private:

	};
}