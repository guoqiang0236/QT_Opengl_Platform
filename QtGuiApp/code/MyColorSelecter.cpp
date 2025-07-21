#include "mycolorselecter.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

MyColorSelecter::MyColorSelecter(const QColor &initialColor, QWidget *parent, const QString &title)
    : QColorDialog(initialColor, parent), m_currentColor(initialColor)
{
    initUI(title);
}

void MyColorSelecter::initUI(const QString &title) {
    // 设置对话框标题
    setWindowTitle(title);
    
    // 启用透明度通道支持 [3,6](@ref)
    setOption(QColorDialog::ShowAlphaChannel, true);
    
    // 添加实时预览区域
    addPreviewLabel();
    
    // 连接信号槽：实时颜色变化时更新预览和内部状态
    connect(this, &QColorDialog::currentColorChanged, this, &MyColorSelecter::onCurrentColorChanged);
}

void MyColorSelecter::addPreviewLabel() {
    // 创建预览标签并添加到布局
    m_previewLabel = new QLabel("实时预览", this);
    m_previewLabel->setMinimumSize(100, 30);
    m_previewLabel->setFrameShape(QFrame::Box);
    m_previewLabel->setAutoFillBackground(true);
    
    // 设置初始预览颜色
    QPalette palette = m_previewLabel->palette();
    palette.setColor(QPalette::Window, m_currentColor);
    m_previewLabel->setPalette(palette);
    
    // 将预览标签插入到对话框底部 [6](@ref)
    layout()->addWidget(m_previewLabel);
}

void MyColorSelecter::onCurrentColorChanged(const QColor &color) {
    if (color.isValid()) {
        m_currentColor = color;
        // 更新预览标签背景色
        QPalette palette = m_previewLabel->palette();
        palette.setColor(QPalette::Window, color);
        m_previewLabel->setPalette(palette);
        emit colorChanged(color); // 触发实时颜色变化信号
    }
}

QColor MyColorSelecter::currentColor() const {
    return m_currentColor;
}

void MyColorSelecter::setCustomColorLibrary(const QVector<QColor> &customColors) {
    // 设置自定义颜色库（最多16个）[3,7](@ref)
    for (int i = 0; i < qMin(customColors.size(), 16); ++i) {
        setCustomColor(i, customColors[i].rgb());
    }
}