#pragma once

#include "MyMaterial.h"
#include "../MyTexture.h"

class MyPhongMaterial : public MyMaterial {
    Q_OBJECT
public:
    MyPhongMaterial() ;
    ~MyPhongMaterial();

public:
    MyTexture* mDiffuse{ nullptr };
	MyTexture* mSpecularMask{ nullptr };
    float mShiness{ 1.0f };
};