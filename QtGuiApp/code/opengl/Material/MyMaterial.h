#pragma once

#include "../../pch.h"
namespace MyOpenGL {
    // ʹ��C++��ö������
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