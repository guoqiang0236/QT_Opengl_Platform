#ifndef MED_IMG_MAINWINDOW_H
#define MED_IMG_MAINWINDOW_H
#include "pch.h"
#include  "opengl/MyGLWidget.h"

class QVTKOpenGLNativeWidget;
class vtkRenderer;

namespace Ui {
    class MainWindow_UI;
}
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void StyleChanged(const QString& style);
    void ShutDown();
    void OnAnimationFinished();
    void Change_CurrentTime();
	void ProgressChanged(int value, int max); // 进度条更新
    void InitRendererPannel(bool flag);
    void SelectColor();

signals:

private:
    void InitSlots();
    void UpdateGUI();
    void UpdateSize();
    void loadStyleSheet(const QString& path); // 动态加载 QSS
    void InitThread();

   
	
private:
    std::mutex m_mutex; // 互斥锁
    std::unique_ptr<Ui::MainWindow_UI> m_ui; 
    MyThread* m_thread; 
    MyThread_Work* m_thread_work; 
    MyThread_Runnable* m_thread_runnable; 
    QTimer* m_current_time; 
    QThread* m_sub;
    QThread* m_numsub; 
    std::unique_ptr<MyProgressDialog> m_progressDialog; // 进度对话框指针


	MyGLWidget* m_glwidget; // OpenGL Widget
};

#endif // MED_IMG_MAINWINDOW_H