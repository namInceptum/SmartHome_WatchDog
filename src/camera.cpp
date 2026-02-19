
////////////////////////////////////////////////
// using opencv to show videostream from webcam
// 
////////////////////////////////////////////////
/**
 *  MIT License
 *  Copyright (c) E. Schilling
 *  See accompanying LICENSE file
 */


#include "camera.h"
#include <QDebug>
#include <QPixmap>

// for yolo object detection
#include "yolo.hpp"
// include telegram to send images if an object is detected
#include "telegram.h"



Camera::Camera(QObject *parent)
    : QObject(parent),
      timer(new QTimer(this)),
      displayLabel(nullptr),
      running(false),
      camWidth(0),
      camHeight(0),
      camFps(0)
{
    connect(timer, &QTimer::timeout, this, &Camera::captureFrame);
}

Camera::~Camera()
{
    stopCamera();
}

bool Camera::startCamera(QLabel *targetLabel, int width, int height, int framerate)
{
    displayLabel = targetLabel;
    camWidth = width;
    camHeight = height;
    camFps = framerate;

    if (!cap.open(0)) { // or !cap.open(0)
        qWarning() << "Failed to open webcam!";
        
        return false;
    }

    // Set resolution & framerate
    cap.set(cv::CAP_PROP_FRAME_WIDTH, camWidth);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, camHeight);
    cap.set(cv::CAP_PROP_FPS, camFps);

    running = true;
    int interval = 1000 / camFps; // cyle interval to meet fps requirement
    timer->start(interval);

    return true;
}

void Camera::stopCamera()
{
    if (running) {
        timer->stop();
        cap.release();
        running = false;
    }
}

bool Camera::isCameraRunning() const
{
    return running;
}

void Camera::captureFrame()
{
    if (!cap.isOpened() || !displayLabel){
        qDebug() << "Either camera not open or displayLabel does not exist";
        return;
    }

    cv::Mat frame;
    cap >> frame; // the current camera frame

    if (frame.empty()){
        return;
    }
    if(rotate180){
        cv::rotate(frame, frame, cv::ROTATE_180);
        //frame = rotated;
        }
        

    // Convert BGR -> RGB
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    // do object detection on the frame
    auto [obj_detected, result_image] = detect(frame);
    frame = result_image;
    QImage image(
        frame.data,
        frame.cols,
        frame.rows,
        frame.step,
        QImage::Format_RGB888
    );
    
    
    if (obj_detected){// we want to detect human, cars, dogs, bicycles, motorcycles
        std::cout << "object detected" << std::endl;
        //frame = result_image;
        if(send_detected_obj){
            qDebug() << "image sending not configured";
            telegramBot.send_frame(image);
        }
    }

    lastFrame = image.copy();  // store a copy for get_frame() // QImage = image or Cv::Mat = frame

    // display live video on QLabel
    emit frameCaptured(lastFrame);
    displayLabel->setPixmap(QPixmap::fromImage(lastFrame));
   
}



// for motion detection
int Camera::compute_frame_difference(const cv::Mat& currentGray, const cv::Mat& prevGray)
{
    if (currentGray.empty() || prevGray.empty())
        return 0; //no frames to compare

    if (currentGray.size() != prevGray.size())
        return 0; // frame must have same dimensions

    cv::Mat diff; // resulting frame
    cv::absdiff(currentGray, prevGray, diff);

    double maxVal;
    cv::minMaxLoc(diff, nullptr, &maxVal);

    return static_cast<int>(maxVal);
}

///////// setter and getter

void Camera::setRotate180(bool rotate){
    this->rotate180 = rotate;
    return;
    }

bool Camera::getRotate180(){
    return this->rotate180;
    }
    
QImage Camera::get_frame() const
{
    return lastFrame;
}
