#pragma once
#include"../pch.h"
#include <QOpenGLFunctions>
namespace MyOpenGL {
    class MyGeometry :public QObject, protected QOpenGLFunctions_4_5_Core {

        Q_OBJECT
    public:
        MyGeometry(QObject* parent = nullptr);
        MyGeometry(
            const std::vector<float>& positions,
            const std::vector<float>& normals,
            const std::vector<float>& uvs,
            const std::vector<unsigned int>& indices,
            QObject* parent = nullptr);
        MyGeometry(
            const std::vector<float>& positions,
            const std::vector<float>& normals,
            const std::vector<float>& colors,
            const std::vector<float>& uvs,
            const std::vector<unsigned int>& indices,
            QObject* parent = nullptr);

        ~MyGeometry();

        static MyGeometry* createBox(float size);
        static MyGeometry* createSphere(float size);
        static MyGeometry* createPlane(float width, float height);
        static MyGeometry* createScreenPlane(float left, float right, float bottom, float top);
        static MyGeometry* createLogoQuad(float width, float height);
        
        GLuint getVao()const { return mVao; }
        uint32_t getIndicesCount() const { return mIndicesCount; }
    private:
        GLuint mVao{ 0 };
        GLuint mPosVbo{ 0 };
        GLuint mUvVbo{ 0 };
        GLuint mNormalVbo{ 0 };
        GLuint mEbo{ 0 };
        GLuint mColorVbo{ 0 };
        uint32_t mIndicesCount{ 0 };
    };
}