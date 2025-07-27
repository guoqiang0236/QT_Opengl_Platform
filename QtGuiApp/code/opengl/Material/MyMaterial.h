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

        //深度检测相关
        bool mDepthTest{ true };
        GLenum mDepthFunc{ GL_LEQUAL };//小于等于的时候才能通过测试
        bool mDepthWrite{ true };
    };
}