#pragma once

#include "MyLight.h"
namespace MyOpenGL {
    class MyDirectionalLight : public MyLight {
    public:
        MyDirectionalLight();
        ~MyDirectionalLight();

    public:
        glm::vec3 mDirection{ -1.0f };
    };
}