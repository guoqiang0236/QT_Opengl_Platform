#pragma once
#include "MyCamera.h"
namespace OpenGLCamera {
    class MyPerspectiveCamera : public MyCamera {
    public:
        MyPerspectiveCamera(float fovy, float aspect, float near, float far, QObject* parent = nullptr);
        ~MyPerspectiveCamera();

        glm::mat4 getProjectionMatrix() override;

        //Ëõ·Å
        void scale(float deltaScale) override;
    private:
        float mFovy = 0.0f;
        float mAspect = 0.0f;
        float mNear = 0.0f;
        float mFar = 0.0f;
    };
}