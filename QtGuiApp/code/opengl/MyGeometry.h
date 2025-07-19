#pragma once
#include"../pch.h"
#include <QOpenGLFunctions>

class MyGeometry :public QObject,protected QOpenGLFunctions_4_5_Core {

    Q_OBJECT
public:
    MyGeometry(QObject* parent = nullptr);
    ~MyGeometry();

    static MyGeometry* createBox(float size);
    static MyGeometry* createSphere(float size);
    static MyGeometry* createPlane(float width ,float height);

    static MyGeometry* createLogoQuad(float width, float height);

	GLuint getVao()const { return mVao; }
	uint32_t getIndicesCount() const { return mIndicesCount; }
private:
    GLuint mVao{ 0 };
    GLuint mPosVbo{ 0 };
    GLuint mUvVao{ 0 };
	GLuint mNormalVbo{ 0 };
    GLuint mEbo{ 0 };

	uint32_t mIndicesCount{ 0 };
};