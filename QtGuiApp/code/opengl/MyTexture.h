#pragma once
#include"../pch.h"
#include <string>
namespace MyOpenGL {
	class MyTexture :protected QOpenGLFunctions_4_5_Core
	{
	public:
		MyTexture(const std::string& path, unsigned int unit);
		MyTexture(unsigned int unit,		//���ص�����Ԫ
			unsigned char* dataIn,	//�ڴ�ָ��
			uint32_t widthIn,		//���
			uint32_t heightIn		//�߶�
		);//���ڴ��ж�ȡͼƬ����ʼ��(����fbx����ģ�͵�ʱ������ͼҲһ����ص��ڴ���)
		~MyTexture();

		static MyTexture* createTexture(const std::string& path, unsigned int unit);
		static MyTexture* createTextureFromMemory(
			const std::string& path,
			unsigned int unit,
			unsigned char* dataIn,	//�ڴ�ָ��
			uint32_t widthIn,		//���
			uint32_t heightIn		//�߶�
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

		//ע��: ��̬!! ������� ������ĳ������ 
		static std::map<std::string, MyTexture*> mTextureCache;
	};
}