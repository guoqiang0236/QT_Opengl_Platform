#pragma once

#include "pch.h"
#include <QObject>
#include <QString>
#include <QImage>
#include <opencv2/opencv.hpp>

class OpenCVImageUtil : public QObject
{
    Q_OBJECT
public:
    enum class ColorConvertType {
        Gray = 0,
        HSV = 1,
        RGB = 2
    };
    explicit OpenCVImageUtil(QObject* parent = nullptr);
    bool openImage(const std::string& filePath);
    QImage getCurrentImage() const;
    cv::Mat getCurrentMat() const;

    // 颜色空间转换接口
    bool convertColor(int type);

private:
    cv::Mat m_mat;
};
