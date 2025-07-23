#include "MyTexture.h"
#include <opencv2/opencv.hpp>
namespace MyOpenGL {
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth = img.cols, mHeight = img.rows,//第一个GL_RGBA告诉 OpenGL ​​如何存储纹理数据​​（在 GPU 内存中的格式）
            0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);//第二个GL_RGBA参数告诉 OpenGL ​​输入数据（img.data）的排列方式​​。

        //opengl生成mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        //手动写mipmap
        //{
        //    int width = img.cols, height = img.rows;
        //    //遍历每个mipmap的层级，为每个级别的mipmap填充图片数据
        //    for (int level = 0; true; ++level) {
        //        //1 将当前级别的mipmap对应的数据发往gpu端
        //        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height,
        //            0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);

        //        //2 判断是否退出循环
        //        if (width == 1 && height == 1) {
        //            break;
        //        }

        //        //3 计算下一次循环的宽度/高度，除以2
        //        width = width > 1 ? width / 2 : 1;
        //        height = height > 1 ? height / 2 : 1;
        //    }
        //}
        img.release();

        //4 设置纹理的过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //用mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        //*****重要*****//
        //GL_NEAREST:在单个mipmap上采用最邻近采样
        //GL_LINEAR   
        //   
        //MIPMAP_LINEAR：在两层mipmap之间采用线性插值
        //MIPMAP_NEAREST
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        //5 设置纹理的包裹方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v

        /*  •	GL_CLAMP_TO_EDGE：坐标超出范围时，采样纹理边缘的颜色，不会重复或镜像。
          •	GL_CLAMP_TO_BORDER：坐标超出范围时，采样指定的边界颜色（需设置 GL_TEXTURE_BORDER_COLOR）。
          •	GL_MIRRORED_REPEAT：超出范围时，纹理坐标以镜像的方式重复。
          •	GL_MIRROR_CLAMP_TO_EDGE（OpenGL 4.4 + ）：超出范围时，先镜像再 clamp 到边缘。*/
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
}