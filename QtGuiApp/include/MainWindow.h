#ifndef MED_IMG_MAINWINDOW_H
#define MED_IMG_MAINWINDOW_H
#include "pch.h"
#include "DcmCStoreSender.h"
#include "MyOpenCVDialog.h"
#include "OpenCVUtil.h"
#include "GLFWApplication.h"
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
    void ControlRecording();
	void ProgressChanged(int value, int max); // 进度条更新
	void InitGLFWWindow();

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

    //DcmtkUtil
	DcmCStoreSender m_dcmCStoreSender;
    //OpenCVUtil
    std::unique_ptr<OpencvUtil> m_opencvUtil;
    std::unique_ptr<MyOpenCVDialog> m_opencvDialog; // OpenCV对话框指针

	
};

#endif // MED_IMG_MAINWINDOW_H