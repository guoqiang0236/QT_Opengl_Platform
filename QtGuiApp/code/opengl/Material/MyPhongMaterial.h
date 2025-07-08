#pragma once

#include "MyMaterial.h"
#include "../MyTexture.h"

class PhongMaterial : public MyMaterial {
    Q_OBJECT
public:
    PhongMaterial() ;
    ~PhongMaterial();

public:
    MyTexture* mDiffuse{ nullptr };
    float mShiness{ 1.0f };
};