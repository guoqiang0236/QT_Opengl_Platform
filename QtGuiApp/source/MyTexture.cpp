#include "MyTexture.h"
#include <opencv2/opencv.hpp>

MyTexture::MyTexture(const std::string& path, unsigned int unit)
{
    initializeOpenGLFunctions();
	mUnit = unit;

    // 1. 用 OpenCV 读取图片（注意：imread 默认是 BGR）
    cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (img.empty()) {
        qDebug() << "图片加载失败";
        return;
    }
    //OpenCV 默认左上角为原点，OpenGL 纹理左下角为原点，需要上下翻转
    cv::flip(img, img, 0);
    //转为 RGBA 格式（如果不是4通道）
    if (img.channels() == 3) {
        cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);
    }
    else if (img.channels() == 4) {
        cv::cvtColor(img, img, cv::COLOR_BGRA2RGBA);
    }

    //2 生成纹理并且激活单元绑定
	glGenTextures(1, &mTexture);
    //--激活纹理单元--
    glActiveTexture(GL_TEXTURE0 + mUnit);
    //--绑定纹理对象--
	glBindTexture(GL_TEXTURE_2D, mTexture);

    //3 传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth = img.cols, mHeight = img.rows,
        0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
    img.release();

    //4 设置纹理的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//5 设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v
}

MyTexture::~MyTexture()
{
}

void MyTexture::bind()
{
    //先切换纹理单元，然后绑定texture对象
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}
