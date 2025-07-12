#pragma once

#include "../../pch.h"

class MyLight {
public:
    MyLight();
    ~MyLight();

public:
    glm::vec3 mColor{ 1.0f };
    float mSpecularIntensity{ 1.0f };
    float mIntensity{ 1.0f };
};