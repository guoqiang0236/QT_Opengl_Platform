/********************************************************************************
** Form generated from reading UI file 'MainWindow-UI.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOW_2D_UI_H
#define MAINWINDOW_2D_UI_H

#include <..\control\imageswitch\imageswitch.h>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow_UI
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frame_menu;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QGridLayout *gridLayout_7;
    QFrame *frame;
    QLabel *label_showthreadnum;
    QPushButton *pushButton_drawtriangle;
    QWidget *tab_4;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_logo;
    ImageSwitch *widget_logo;
    QLabel *label_logo;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout_object;
    ImageSwitch *widget_object;
    QLabel *label_object;
    QWidget *tab;
    QComboBox *comboBox;
    QLabel *label_5;
    QPushButton *pushButton_opencvtest;
    QFrame *frame_2;
    QPushButton *pushButton_shutdown;
    QFrame *frame_vtkrender;
    QGridLayout *gridLayout_2;
    QFrame *frame_main;
    QGridLayout *gridLayout_8;
    QFrame *frame_3;
    QFrame *frame1;
    QHBoxLayout *horizontalLayout;
    QLabel *label_hospital;
    QLabel *label_currenttime;
    QOpenGLWidget *openGLWidget;

    void setupUi(QMainWindow *MainWindow_UI)
    {
        if (MainWindow_UI->objectName().isEmpty())
            MainWindow_UI->setObjectName("MainWindow_UI");
        MainWindow_UI->resize(1395, 897);
        centralwidget = new QWidget(MainWindow_UI);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        frame_menu = new QFrame(centralwidget);
        frame_menu->setObjectName("frame_menu");
        frame_menu->setMaximumSize(QSize(208, 16777215));
        frame_menu->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: transparent; /* \350\203\214\346\231\257\351\200\217\346\230\216 */\n"
"    border: none;                /* \347\247\273\351\231\244\350\276\271\346\241\206 */\n"
"}"));
        frame_menu->setFrameShape(QFrame::Shape::StyledPanel);
        frame_menu->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(frame_menu);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(frame_menu);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setAutoFillBackground(true);
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        gridLayout_7 = new QGridLayout(tab_3);
        gridLayout_7->setObjectName("gridLayout_7");
        gridLayout_7->setContentsMargins(-1, 0, -1, -1);
        frame = new QFrame(tab_3);
        frame->setObjectName("frame");
        frame->setEnabled(false);
        frame->setMaximumSize(QSize(16777215, 200));
        frame->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: transparent; /* \350\203\214\346\231\257\351\200\217\346\230\216 */\n"
"    border: none;                /* \347\247\273\351\231\244\350\276\271\346\241\206 */\n"
"}"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);

        gridLayout_7->addWidget(frame, 2, 1, 1, 1);

        label_showthreadnum = new QLabel(tab_3);
        label_showthreadnum->setObjectName("label_showthreadnum");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_showthreadnum->sizePolicy().hasHeightForWidth());
        label_showthreadnum->setSizePolicy(sizePolicy);
        label_showthreadnum->setMaximumSize(QSize(200, 100));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        label_showthreadnum->setFont(font);

        gridLayout_7->addWidget(label_showthreadnum, 1, 1, 1, 1);

        pushButton_drawtriangle = new QPushButton(tab_3);
        pushButton_drawtriangle->setObjectName("pushButton_drawtriangle");

        gridLayout_7->addWidget(pushButton_drawtriangle, 0, 1, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        gridLayout_3 = new QGridLayout(tab_4);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_logo = new QGridLayout();
        gridLayout_logo->setSpacing(0);
        gridLayout_logo->setObjectName("gridLayout_logo");
        gridLayout_logo->setContentsMargins(-1, 10, -1, 10);
        widget_logo = new ImageSwitch(tab_4);
        widget_logo->setObjectName("widget_logo");
        widget_logo->setMinimumSize(QSize(100, 25));
        widget_logo->setMaximumSize(QSize(100, 25));

        gridLayout_logo->addWidget(widget_logo, 0, 1, 1, 1);

        label_logo = new QLabel(tab_4);
        label_logo->setObjectName("label_logo");
        label_logo->setMinimumSize(QSize(50, 25));
        label_logo->setMaximumSize(QSize(200, 25));

        gridLayout_logo->addWidget(label_logo, 0, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_logo, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 0, 1, 1);

        gridLayout_object = new QGridLayout();
        gridLayout_object->setSpacing(0);
        gridLayout_object->setObjectName("gridLayout_object");
        gridLayout_object->setContentsMargins(-1, 10, -1, 10);
        widget_object = new ImageSwitch(tab_4);
        widget_object->setObjectName("widget_object");
        widget_object->setMinimumSize(QSize(100, 25));
        widget_object->setMaximumSize(QSize(100, 25));

        gridLayout_object->addWidget(widget_object, 0, 1, 1, 1);

        label_object = new QLabel(tab_4);
        label_object->setObjectName("label_object");
        label_object->setMinimumSize(QSize(50, 25));
        label_object->setMaximumSize(QSize(200, 25));

        gridLayout_object->addWidget(label_object, 0, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_object, 0, 0, 1, 1);

        tabWidget->addTab(tab_4, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        comboBox = new QComboBox(tab);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(70, 20, 111, 24));
        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(3, 20, 61, 21));
        pushButton_opencvtest = new QPushButton(tab);
        pushButton_opencvtest->setObjectName("pushButton_opencvtest");
        pushButton_opencvtest->setGeometry(QRect(40, 100, 121, 28));
        frame_2 = new QFrame(tab);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(40, 390, 120, 80));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_shutdown = new QPushButton(tab);
        pushButton_shutdown->setObjectName("pushButton_shutdown");
        pushButton_shutdown->setGeometry(QRect(50, 210, 100, 100));
        pushButton_shutdown->setMinimumSize(QSize(100, 100));
        pushButton_shutdown->setMaximumSize(QSize(100, 100));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::SystemShutdown));
        pushButton_shutdown->setIcon(icon);
        pushButton_shutdown->setIconSize(QSize(100, 100));
        tabWidget->addTab(tab, QString());

        verticalLayout->addWidget(tabWidget);


        gridLayout->addWidget(frame_menu, 0, 0, 1, 1);

        frame_vtkrender = new QFrame(centralwidget);
        frame_vtkrender->setObjectName("frame_vtkrender");
        frame_vtkrender->setMinimumSize(QSize(0, 0));
        frame_vtkrender->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: transparent; /* \350\203\214\346\231\257\351\200\217\346\230\216 */\n"
"    border: none;                /* \347\247\273\351\231\244\350\276\271\346\241\206 */\n"
"}"));
        frame_vtkrender->setFrameShape(QFrame::Shape::StyledPanel);
        frame_vtkrender->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_2 = new QGridLayout(frame_vtkrender);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_main = new QFrame(frame_vtkrender);
        frame_main->setObjectName("frame_main");
        frame_main->setFrameShape(QFrame::Shape::StyledPanel);
        frame_main->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_8 = new QGridLayout(frame_main);
        gridLayout_8->setObjectName("gridLayout_8");
        frame_3 = new QFrame(frame_main);
        frame_3->setObjectName("frame_3");
        frame_3->setMaximumSize(QSize(16777215, 50));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);

        gridLayout_8->addWidget(frame_3, 2, 0, 1, 1);

        frame1 = new QFrame(frame_main);
        frame1->setObjectName("frame1");
        frame1->setMaximumSize(QSize(16777215, 100));
        horizontalLayout = new QHBoxLayout(frame1);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        label_hospital = new QLabel(frame1);
        label_hospital->setObjectName("label_hospital");
        label_hospital->setMinimumSize(QSize(300, 100));
        label_hospital->setMaximumSize(QSize(16777215, 100));
        label_hospital->setFont(font);
        label_hospital->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_hospital);

        label_currenttime = new QLabel(frame1);
        label_currenttime->setObjectName("label_currenttime");
        label_currenttime->setMinimumSize(QSize(300, 100));
        label_currenttime->setMaximumSize(QSize(16777215, 100));
        label_currenttime->setFont(font);
        label_currenttime->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_currenttime);


        gridLayout_8->addWidget(frame1, 0, 0, 1, 1);

        openGLWidget = new QOpenGLWidget(frame_main);
        openGLWidget->setObjectName("openGLWidget");

        gridLayout_8->addWidget(openGLWidget, 1, 0, 1, 1);


        gridLayout_2->addWidget(frame_main, 0, 0, 1, 1);


        gridLayout->addWidget(frame_vtkrender, 0, 1, 1, 1);

        MainWindow_UI->setCentralWidget(centralwidget);
#if QT_CONFIG(shortcut)
        label_5->setBuddy(comboBox);
#endif // QT_CONFIG(shortcut)

        retranslateUi(MainWindow_UI);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow_UI);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow_UI)
    {
        MainWindow_UI->setWindowTitle(QCoreApplication::translate("MainWindow_UI", "MainWindow", nullptr));
        label_showthreadnum->setText(QCoreApplication::translate("MainWindow_UI", "0", nullptr));
        pushButton_drawtriangle->setText(QCoreApplication::translate("MainWindow_UI", "drawtriangle", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow_UI", "\346\202\243\350\200\205\344\277\241\346\201\257", nullptr));
        label_logo->setText(QCoreApplication::translate("MainWindow_UI", "LOGO\346\230\276\347\244\272", nullptr));
        label_object->setText(QCoreApplication::translate("MainWindow_UI", "\346\234\254\344\275\223\346\230\276\347\244\272", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow_UI", "\346\270\262\346\237\223\344\272\244\344\272\222", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow_UI", "ElegantDark", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow_UI", "AMOLED", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow_UI", "Aqua", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow_UI", "ConsoleStyle", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("MainWindow_UI", "MacOS", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("MainWindow_UI", "ManjaroMix", nullptr));
        comboBox->setItemText(6, QCoreApplication::translate("MainWindow_UI", "MaterialDark", nullptr));
        comboBox->setItemText(7, QCoreApplication::translate("MainWindow_UI", "NeonButtons", nullptr));
        comboBox->setItemText(8, QCoreApplication::translate("MainWindow_UI", "Ubuntu", nullptr));

        label_5->setText(QCoreApplication::translate("MainWindow_UI", "\344\270\273\351\242\230", nullptr));
        pushButton_opencvtest->setText(QCoreApplication::translate("MainWindow_UI", "OpenCVtest", nullptr));
        pushButton_shutdown->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow_UI", "\347\263\273\347\273\237\350\256\276\347\275\256", nullptr));
        label_hospital->setText(QCoreApplication::translate("MainWindow_UI", "TextLabel", nullptr));
        label_currenttime->setText(QCoreApplication::translate("MainWindow_UI", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow_UI: public Ui_MainWindow_UI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOW_2D_UI_H
