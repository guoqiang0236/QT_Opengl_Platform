#pragma once

#include "MyCamera.h"
namespace OpenGLCamera {
    class MyCameraControl :public QObject {
        Q_OBJECT
    public:
        MyCameraControl(QObject* parent = nullptr);
        virtual ~MyCameraControl();

        // 事件处理接口（虚函数）
        virtual void onMouse(int button, int action, double xpos, double ypos);
        virtual void onCursor(double xpos, double ypos);
        virtual void onKey(int key, int action, int mods);
        virtual void onScroll(double offset);

        // 每一帧渲染之前都要进行调用，每一帧更新的行为可以放在这里
        virtual void update();

        void setcamera(MyCamera* camera) { mCamera = camera; }
        void setSensitivity(float sensitivity) { mSensitivity = sensitivity; }
        void setScaleSpeed(float scaleSpeed) { mScaleSpeed = scaleSpeed; }

    protected:
        // 1. 鼠标按键状态
        bool mLeftMouseDown = false;
        bool mRightMouseDown = false;
        bool mMiddleMouseDown = false;

        // 2. 当前鼠标的位置
        float mCurrentX = 0.0f;
        float mCurrentY = 0.0f;

        // 3. 敏感度
        float mSensitivity = 1.0f;

        // 4. 记录键盘相关按键的相关状态
        std::map<int, bool> mKeyMap;

        // 5. 存储一个MyCamera对象、
        MyCamera* mCamera = nullptr;

        // 6. 记录相机缩放的速度
        float mScaleSpeed = 1.0f; // 缩放速度
    };

}