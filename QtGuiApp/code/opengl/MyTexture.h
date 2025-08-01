#pragma once
#include"../pch.h"
#include <string>
namespace MyOpenGL {
	class MyTexture :protected QOpenGLFunctions_4_5_Core
	{
	public:
		MyTexture(const std::string& path, unsigned int unit);
		MyTexture(unsigned int unit,		//挂载的纹理单元
			unsigned char* dataIn,	//内存指针
			uint32_t widthIn,		//宽度
			uint32_t heightIn		//高度
		);//从内存中读取图片并初始化(部分fbx加载模型的时候会把贴图也一起加载到内存中)
		~MyTexture();

		static MyTexture* createTexture(const std::string& path, unsigned int unit);
		static MyTexture* createTextureFromMemory(
			const std::string& path,
			unsigned int unit,
			unsigned char* dataIn,	//内存指针
			uint32_t widthIn,		//宽度
			uint32_t heightIn		//高度
		);

		void bind();

		void setUnit(unsigned int unit) { mUnit = unit; }

		int getWidth()const { return mWidth; }
		int getHeight()const { return mHeight; }
		GLuint getTexture()const { return mTexture; }

	private:
		GLuint mTexture{ 0 };
		int mWidth{ 0 };
		int mHeight{ 0 };
		unsigned int mUnit{ 0 };

		//注意: 静态!! 属于类的 不属于某个对象 
		static std::map<std::string, MyTexture*> mTextureCache;
	};
}