#pragma once

#include "../../pch.h"
namespace MyOpenGL {
    // ʹ��C++��ö������
    enum class MaterialType {
        PhongMaterial,
        WhiteMaterial,
        ImageMaterial,
        DepthMaterial,
        OpacityMaskMaterial,
        ScreenMaterial,
        CubeMaterial,
        PhongEnvMaterial,
        PhongInstanceMaterial,
        GrassInstanceMaterial
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

        //stencil���
        bool mStencilTest{ false };
        unsigned int mSFail{ GL_KEEP };//ģ�����ʧ������ô��
        unsigned int mZFail{ GL_KEEP };//ģ�����ͨ��������Ȳ���ûͨ����ô��
        unsigned int mZPass{ GL_KEEP };//ģ��+��Ȳ��Զ�ͨ������ô��

        unsigned int mStencilMask{ 0xff };//���ڿ���ģ��д��

        unsigned int mStencilFunc{ GL_ALWAYS };
        unsigned int mStencilRef{ 0 };
        unsigned int mStencilFuncMask{ 0xff };

        //��ɫ���
		bool mBlend{ false };
		unsigned int mSFactor{ GL_SRC_ALPHA };
        unsigned int mDFactor{ GL_ONE_MINUS_SRC_ALPHA };

        //face culling
        bool mFaceCulling{ false };
        unsigned int mFrontFace{ GL_CCW };
        unsigned int mCullFace{ GL_BACK };

        float mOpacity{ 1.0f };
    };
}