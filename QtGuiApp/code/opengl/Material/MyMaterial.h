#pragma once

#include "../../pch.h"

// 使用C++的枚举类型
enum class MaterialType {
    PhongMaterial
};

class MyMaterial :public QObject {
    Q_OBJECT
public:
    MyMaterial();
    ~MyMaterial();
public:
    MaterialType mType;
};