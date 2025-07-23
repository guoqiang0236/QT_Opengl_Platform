#pragma once

#include "MyMaterial.h"
#include "../MyTexture.h"
namespace MyOpenGL {
    class MyImageMaterial : public MyMaterial {
        Q_OBJECT
    public:
        MyImageMaterial();
        ~MyImageMaterial();

    public:
        MyOpenGL::MyTexture* mDiffuse{ nullptr };
        MyOpenGL::MyTexture* mSpecularMask{ nullptr };
        float mShiness{ 1.0f };
    };
}