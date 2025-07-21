#ifndef MYCOLORSELECTER_H
#define MYCOLORSELECTER_H

#include <QColorDialog>
#include <QObject>
#include <QVector>

class MyColorSelecter : public QColorDialog
{
    Q_OBJECT

public:
    // 构造函数：支持设置初始颜色、父窗口和对话框标题
    explicit MyColorSelecter(const QColor &initialColor = Qt::white, 
                             QWidget *parent = nullptr, 
                             const QString &title = "选择颜色");

    // 获取当前选中的颜色
    QColor currentColor() const;

    // 设置自定义颜色库（最多支持16个颜色）
    void setCustomColorLibrary(const QVector<QColor> &customColors);

signals:
    // 信号：当用户最终选择颜色时触发（点击确定）
    void colorSelected(const QColor &color);
    // 信号：实时颜色变化时触发（滑动选择时）
    void colorChanged(const QColor &color);

private slots:
    // 内部槽：处理实时颜色变化，更新预览
    void onCurrentColorChanged(const QColor &color);

private:
    // 初始化UI和信号槽连接
    void initUI(const QString &title);
    // 添加实时预览区域
    void addPreviewLabel();

    QColor m_currentColor; // 当前颜色
    QLabel *m_previewLabel; // 预览标签
};

#endif // MYCOLORSELECTER_H