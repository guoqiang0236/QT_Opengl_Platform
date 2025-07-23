#pragma once

#include "MyLight.h"
namespace MyOpenGL {
    class MyAmbientLight : public MyLight {
    public:
        MyAmbientLight();
        ~MyAmbientLight();
    };
}