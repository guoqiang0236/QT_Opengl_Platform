#pragma once
#include "MyCamera.h"

namespace OpenGLCamera {
    class MyOrthographicCamera : public MyCamera
    {
    public:
        MyOrthographicCamera(float l, float r, float t, float b, float n, float f, QObject* parent = nullptr);
        ~MyOrthographicCamera();
        glm::mat4 getProjectionMatrix()  override;

        //Ëõ·Å
        void scale(float deltaScale) override;
    private:
        float mLeft = 0.0f;
        float mRight = 0.0f;
        float mTop = 0.0f;
        float mBottom = 0.0f;
        float mNear = 0.0f;
        float mFar = 0.0f;

        float mScaleFactor = 0.0f;
    };
}