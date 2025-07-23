#pragma once

#include "MyCameraControl.h"
namespace OpenGLCamera {
    class MyTrackBallCameraControl : public MyCameraControl
    {
    public:
        MyTrackBallCameraControl(QObject* parent = nullptr);
        ~MyTrackBallCameraControl();

        // ��д�������¼�������
        void onCursor(double xpos, double ypos) override;

        void onScroll(double offset) override;
    private:
        void rotatePitch(float angle);
        void rotateYaw(float angle);

    private:
        float mMoveSpeed = 0.01f;

    };
}
