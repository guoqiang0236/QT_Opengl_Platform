#include "MyCameraControl.h"


namespace OpenGLCamera {
    MyCameraControl::MyCameraControl(QObject* parent)
    {
    }

    MyCameraControl::~MyCameraControl()
    {
    }

    void MyCameraControl::onMouse(int button, int action, double xpos, double ypos)
    {
        // 1. 判断当前的按键是否按下（使用GLFW常量）
        bool pressed = (action == 1); // 简化的布尔表达式

        // 2. 如果按下，记录当前按下的位置
        if (pressed) {
            mCurrentX = static_cast<float>(xpos); // 图片中显示需要存储为float
            mCurrentY = static_cast<float>(ypos);
        }

        // 3. 根据按下的鼠标按键不同执行后续操作
        switch (button) {
        case 1:
            mLeftMouseDown = pressed;
            break;

        case 2:
            mMiddleMouseDown = pressed;
            break;
        }
    }

    void MyCameraControl::onCursor(double xpos, double ypos)
    {

    }

    void MyCameraControl::onKey(int key, int action, int mods)
    {
        // 1. 检测按下或者抬起，给到一个变量
        bool pressed = (action == 1);

        // 2. 记录在keyMap
        mKeyMap[key] = pressed;
    }

    void MyCameraControl::onScroll(double offset)
    {
    }

    void MyCameraControl::update()
    {
    }

}
