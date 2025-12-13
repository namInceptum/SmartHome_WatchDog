/********************************************************************************
** Definition of all required visual buttons and lables as well as corresponding variables
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QVideoWidget>

#include <QCameraDevice>
#include <QPalette>
#include <QPushButton>


QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    // main window as widget
    QWidget *centralWidget;

    //define button objects
    QPushButton *send_frame_btn;
    QPushButton *cancel_app_btn;

    //QVideoWidget *videoWidget;  // Define video widget
    QLabel *videoLabel;
    
    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");

        MainWindow->resize(800, 480); // screen dimension rspi
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");


        
        //// define buttons and menu options 

        // font for buttons:
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        // button to send image
        send_frame_btn = new QPushButton(centralWidget);
        send_frame_btn->setObjectName("send_frame_btn");
        send_frame_btn->setGeometry(QRect(632, 110, 150, 30));
        send_frame_btn->setFont(font1);
        send_frame_btn->setText("Send image");
        // css code to set the style of the button
        send_frame_btn->setStyleSheet(R"(
        QPushButton {
        font-size: 14px;
        color: white;
        background-color: #1B8536;
        })");
        
        // exit the application button
        cancel_app_btn = new QPushButton("Exit App", centralWidget);
        cancel_app_btn->setObjectName("cancel_app_btn");
        cancel_app_btn->setGeometry(QRect(632, 150, 150, 30));  // x, y, width , hight
        cancel_app_btn->setFont(font1);
        cancel_app_btn->setStyleSheet(R"(
        QPushButton {
        font-size: 14px;
        color: white;
        background-color: #1B8536;
        })");


        ////// show the video stream on a label from the camera
        videoLabel = new QLabel("Home WebCam", centralWidget);
        videoLabel->setGeometry(20, 110, 580, 360);
        videoLabel->setStyleSheet("background-color: black;");
        videoLabel->setScaledContents(true);

        
        // add the central widget to the mainwindow object
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);
        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        // this retranslateUI function is usefull if you want to provide multilanguage application
        // for the OAT english is ok, such we just hardcode the language above with the button and label definitions 
        // like help_button->setText("Help");
    };

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
