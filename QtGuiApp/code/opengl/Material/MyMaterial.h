#pragma once

#include "../../pch.h"
namespace MyOpenGL {
    // 使用C++的枚举类型
    enum class MaterialType {
        PhongMaterial,
        WhiteMaterial,
        ImageMaterial,
        DepthMaterial
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

        //polygonOffset相关
        bool mPolygonOffset{ false };
        unsigned int mPolygonOffsetType{ GL_POLYGON_OFFSET_FILL };
        float mFactor{ 0.0f };
        float mUnit{ 0.0f };
    };
}