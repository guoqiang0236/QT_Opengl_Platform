#pragma once

#include "MyMaterial.h"
#include "../MyTexture.h"
namespace MyOpenGL {
    class MyPhongMaterial : public MyMaterial {
        Q_OBJECT
    public:
        MyPhongMaterial();
        ~MyPhongMaterial();

    public:
        MyOpenGL::MyTexture* mDiffuse{ nullptr };
        MyOpenGL::MyTexture* mSpecularMask{ nullptr };
        float mShiness{ 1.0f };
    };
}