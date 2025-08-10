#pragma once

#include "../MyObject.h"
#include "../MyGeometry.h"
#include "../material/MyMaterial.h"
namespace MyOpenGL {
    class MyMesh : public MyObject {
    public:
        MyMesh(MyGeometry* geometry, MyMaterial* material);
        ~MyMesh();

    public:
        MyGeometry* mGeometry{ nullptr };
        MyMaterial* mMaterial{ nullptr };
    };
}