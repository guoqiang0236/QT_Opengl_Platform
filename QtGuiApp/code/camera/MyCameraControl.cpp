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
        // 1. �жϵ�ǰ�İ����Ƿ��£�ʹ��GLFW������
        bool pressed = (action == 1); // �򻯵Ĳ������ʽ

        // 2. ������£���¼��ǰ���µ�λ��
        if (pressed) {
            mCurrentX = static_cast<float>(xpos); // ͼƬ����ʾ��Ҫ�洢Ϊfloat
            mCurrentY = static_cast<float>(ypos);
        }

        // 3. ���ݰ��µ���갴����ִͬ�к�������
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
        // 1. ��ⰴ�»���̧�𣬸���һ������
        bool pressed = (action == 1);

        // 2. ��¼��keyMap
        mKeyMap[key] = pressed;
    }

    void MyCameraControl::onScroll(double offset)
    {
    }

    void MyCameraControl::update()
    {
    }

}
