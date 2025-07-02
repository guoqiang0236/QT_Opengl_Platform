#pragma once
#include"../pch.h"
#include <QOpenGLFunctions>

class MyGeometry :protected QOpenGLFunctions_4_5_Core {
public:
    MyGeometry();
    ~MyGeometry();

    static MyGeometry* createBox(float size);
    static MyGeometry* createSphere(float size);

	GLuint getVao()const { return mVao; }
	uint32_t getIndicesCount() const { return mIndicesCount; }
private:
    GLuint mVao{ 0 };
    GLuint mPosVbo{ 0 };
    GLuint mUvVao{ 0 };
    GLuint mEbo{ 0 };

	uint32_t mIndicesCount{ 0 };
};