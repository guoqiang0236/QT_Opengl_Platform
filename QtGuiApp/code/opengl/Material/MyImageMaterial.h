#pragma once

#include "MyMaterial.h"
#include "../MyTexture.h"

class MyImageMaterial : public MyMaterial {
    Q_OBJECT
public:
    MyImageMaterial() ;
    ~MyImageMaterial();

public:
    MyTexture* mDiffuse{ nullptr };
	MyTexture* mSpecularMask{ nullptr };
    float mShiness{ 1.0f };
};