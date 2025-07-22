#ifndef PCH_H
#define PCH_H

// 添加常用的头文件
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// Qt 相关头文件
#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QIcon>
#include <QSlider>
#include <QPushButton>
#include <QComboBox>
#include <QQuickWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <QDateTime>
#include <QTimer>
#include <QThreadPool>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core> 
#include <QObject>
#include <QWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QElapsedTimer>
#include <QSerialPort>

//自定义Qt类
#include "thread/MyThread.h"
#include "thread/MyThread_Work.h"
#include "thread/MyThread_QRunnable.h"
#include "MyProgressDialog.h"

// VTK 相关头文件
#include <vtkCamera.h>
#include <vtkImageReslice.h>
#include <vtkMatrix4x4.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkMarchingCubes.h>
#include <vtkRenderer.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkAbstractVolumeMapper.h>  
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkTextProperty.h>
#include <vtkGPUVolumeRayCastMapper.h>  
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCubeAxesActor.h>
#include <vtkImageReader.h>
#include <vtkImageViewer2.h>

//全局变量
#include "GlobalConfig.h"
#include "checkError.h"

//GLM]

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#endif // PCH_H