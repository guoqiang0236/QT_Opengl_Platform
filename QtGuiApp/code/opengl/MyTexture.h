#pragma once
#include"../pch.h"
#include <string>
namespace MyOpenGL {
	class MyTexture :protected QOpenGLFunctions_4_5_Core
	{
	public:
		MyTexture(const std::string& path, unsigned int unit);
		~MyTexture();

		void bind();

	private:
		GLuint mTexture{ 0 };
		int mWidth{ 0 };
		int mHeight{ 0 };
		unsigned int mUnit{ 0 };
	};
}