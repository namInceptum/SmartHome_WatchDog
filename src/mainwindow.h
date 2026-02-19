/**
 *  MIT License
 *  Copyright (c) E.Schilling
 *  See accompanying LICENSE file
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QMessageBox>
#include <QProcess>
#include <QGraphicsOpacityEffect>
#include <QtWidgets>

#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>

#include <unistd.h>


#include "camera.h"
#include "telegram.h"
#include <opencv2/opencv.hpp> // for image processing

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr); //const std::string& token, int64_t chatId,
    ~MainWindow();
        
    Camera *camera;

    // private variables
private: 
    Ui::MainWindow *ui;

    //Config cfg;


private:
    QPalette comboBoxPalette;
    
    QMediaCaptureSession *captureSession;
    QVideoWidget *videoWidget;

    // if the camera fails to start initially try to restart
    void on_send_frame_btn();

    // control frame orientation, here flipping option only vertically
    void on_cancel_app_btn();

    
    

};

#endif // MAINWINDOW_H
