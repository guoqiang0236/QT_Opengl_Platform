#include "MainWindow.h"
// 移除不存在的头文件
#include <vtkImageData.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <QDebug>
#include <QIcon>
#include <QFileDialog>
#include <memory>
#include "MainWindow-UI.h"
#include <thread>
#include <iostream>
#include <vtkImageFlip.h>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_ui(std::make_unique<Ui::MainWindow_UI>()),
    m_thread(new MyThread(this)),
    m_thread_work(new MyThread_Work(this)),
	m_thread_runnable(new MyThread_Runnable(this)),
    m_sub(new QThread(this)),
    m_numsub(new QThread(this)),
    m_opencvUtil(std::make_unique<OpencvUtil>()),
    m_glfwHelper (std::make_unique<GLFWHelper>(this))
{

    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/res/icon/favicon.ico")); // 覆盖可能的默认值
    m_ui->setupUi(this);
    InitSlots();
    UpdateSize();
    UpdateGUI();
    InitThread();
}

MainWindow::~MainWindow() 
{
    if (m_sub)
    {
        if (m_sub->isRunning())
        {
            m_sub->quit();
            m_sub->wait();
        }
    }
    if (m_numsub)
    {
        if (m_numsub->isRunning())
        {
            m_numsub->quit();
            m_numsub->wait();
        }
    }
};




void MainWindow::StyleChanged(const QString& style)
{
    qDebug() << "当前选中文本：" << style;
    QString styledir = ":/res/QSS/" + style + ".qss";
    loadStyleSheet(styledir);
}
















void MainWindow::ShutDown()
{
    QApplication::quit();
}

void MainWindow::OnAnimationFinished()
{
}

void MainWindow::Change_CurrentTime()
{
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
	m_ui->label_currenttime->setText(currentTime);
}

void MainWindow::ControlRecording()
{
    if(!m_opencvUtil)
		m_opencvUtil = std::make_unique<OpencvUtil>();

	
	if (!m_opencvUtil->GetIsRecording())
	{
        // 检查并创建 D:\data 文件夹
        QString folderPath = "D:/data";
        QDir dir(folderPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
		std::string outputPath = folderPath.toStdString() + "/output.avi";
        // 获取当前屏幕区域
        RECT rect;
        HWND hwnd = reinterpret_cast<HWND>(this->winId());
        GetWindowRect(hwnd, &rect);
        // 设置录制区域为当前窗口
        m_opencvUtil->StartRecording(rect, outputPath.c_str());
	}
	else
	{
        m_opencvUtil->StopRecording();
	}
}

void MainWindow::ProgressChanged(int value, int max)
{
	if (m_progressDialog)
	{
		m_progressDialog->setProgress(value);
		m_progressDialog->setMaximumValue(max);
		m_progressDialog->setLabelText(QString("正在处理... %1/%2").arg(value).arg(max));
		qApp->processEvents();
	}
}

void MainWindow::InitGLFWWindow()
{
    if (!m_glfwHelper)
        return;
	m_glfwHelper->Initialize();
    m_glfwHelper->CreateOpenglWindow();
    m_glfwHelper->OpenglWindowExec();
    m_glfwHelper->TerminateOpenglWindow();
}



void MainWindow::InitSlots()
{
	m_current_time = new QTimer(this);
	connect(m_current_time, &QTimer::timeout, this, &MainWindow::Change_CurrentTime);
    m_current_time->start(1000);
    
    connect(m_ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::StyleChanged);
    connect(m_ui->pushButton_shutdown, &QPushButton::clicked, this, &MainWindow::ShutDown);
	connect(m_ui->pushButton_record, &QPushButton::clicked, this, &MainWindow::ControlRecording);
  

    //testopencv
	connect(m_ui->pushButton_opencvtest, &QPushButton::clicked, this, [this]() {
        if (m_opencvDialog)
        {
            m_opencvDialog->exec(); // 模态对话框
		}   
		});

    //GLFW
    connect(m_ui->pushButton_testglfw, &QPushButton::clicked, this, &MainWindow::InitGLFWWindow);
}

void MainWindow::UpdateGUI()
{
    if (!m_ui )
        return;
	m_ui->label_hospital->setText("Opengl学习平台");
    m_ui->comboBox->setCurrentIndex(8);
    m_opencvDialog = std::make_unique<MyOpenCVDialog>(this);
    m_opencvDialog->setWindowModality(Qt::ApplicationModal);
}

void MainWindow::UpdateSize()
{
	sysconfig::GlobalConfig& config = sysconfig::GlobalConfig::get();
    QSize windowsize = config.getAdjustedSize();
    this->resize(windowsize);
}

void MainWindow::loadStyleSheet(const QString& path)
{
    QFile qssFile(path);
    if (!qssFile.exists()) {
        qWarning() << "QSS 文件不存在:" << path;
        return;
    }
    if (qssFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(styleSheet); // 全局应用样式
        qssFile.close();
    }
    else {
        qWarning() << "无法打开 QSS 文件:" << path;
    }
}

void MainWindow::InitThread()
{
    //方式一QThread
    //int idealThreads = MyThread::idealThreadCount(); // 获取硬件支持的最大线程数
    //int activeThreads = MyThread::getActiveThreadCount();
    //qDebug() << "硬件支持的最大线程数: " << idealThreads;
    //qDebug() << "现在活跃的线程数: " << activeThreads;

    //if (!m_thread->isRunning())
    //{
    //    m_thread->setPriority(QThread::LowestPriority); // 设置线程优先级
    //    m_thread->start(); // 启动线程
    //}
    //else
    //{
    //    qWarning() << "线程正在运行，无法启动新任务！";
    //}
  /*  connect(m_thread, &MyThread::numberGenerated, this, [this](int num) {
        m_ui->label_showthreadnum->setText(QString::number(num));
        });*/

    //方式二QObject
     // 创建线程对象
    
    if (m_thread_work)
    {
        m_thread_work->moveToThread(m_numsub);
    }
    connect(m_thread_work, &MyThread_Work::numberGenerated, this, [this](int num) {
        m_ui->label_showthreadnum->setText(QString::number(num));
        });
    connect(m_numsub, &QThread::started, m_thread_work, &MyThread_Work::working);
    m_numsub->start();
	
        
   

    //方式三 线程池
    /*QThreadPool::globalInstance()->setMaxThreadCount(4);
	if (m_thread_runnable)
	{
		QThreadPool::globalInstance()->start(m_thread_runnable);
	}
    connect(m_thread_runnable, &MyThread_Runnable::numberGenerated, this, [this](int num) {

        m_ui->label_showthreadnum->setText(QString::number(num));
        });
    */


}


