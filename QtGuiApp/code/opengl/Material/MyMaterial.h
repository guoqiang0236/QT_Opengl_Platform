#pragma once

#include "../../pch.h"
namespace MyOpenGL {
    // 使用C++的枚举类型
    enum class MaterialType {
        PhongMaterial,
        WhiteMaterial,
        ImageMaterial
    };

    class MyMaterial :public QObject {
        Q_OBJECT
    public:
        MyMaterial();
        ~MyMaterial();
    public:
        MaterialType mType;
    };
}