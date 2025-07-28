#pragma once

#include "../../pch.h"
namespace MyOpenGL {
    // ʹ��C++��ö������
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

        //��ȼ�����
        bool mDepthTest{ true };
        GLenum mDepthFunc{ GL_LEQUAL };//С�ڵ��ڵ�ʱ�����ͨ������
        bool mDepthWrite{ true };

        //polygonOffset���
        bool mPolygonOffset{ false };
        unsigned int mPolygonOffsetType{ GL_POLYGON_OFFSET_FILL };
        float mFactor{ 0.0f };
        float mUnit{ 0.0f };
    };
}