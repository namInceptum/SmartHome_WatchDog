/**
 *  MIT License
 *  Copyright (c) E.Schilling
 *  See accompanying LICENSE file
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QImage>
#include <opencv2/opencv.hpp>

#include "yolo.hpp" // include yolo object detection

class Camera : public QObject
{
    Q_OBJECT

public:
    explicit Camera(QObject *parent = nullptr);
    ~Camera();
    // start camera with preferred setup
    bool startCamera(QLabel *targetLabel, int width = 640, int height = 480, int framerate = 30);
    void stopCamera();

    bool isCameraRunning() const;

    // Get current frame (as QImage)
    QImage get_frame() const;
    void setRotate180(bool rotate);
    bool getRotate180();
    
    bool send_detected_obj = true; // send frame if an object is detected
    bool rotate180 = false; // rotate image

signals:
    void frameCaptured(const QImage &image);   // emitted each frame

private slots:
    void captureFrame();

private:
    QTimer *timer;
    QLabel *displayLabel;

    cv::VideoCapture cap; // for video stream
    bool running; // checking if camera is running 
    cv::Mat previous_frame; // used to compare last and current frame for motion detection
    
    int camWidth;
    int camHeight;
    int camFps;
    int compute_frame_difference(const cv::Mat& currentGray,const cv::Mat& prevGray);

    

    QImage lastFrame;  // store latest frame
};

#endif // CAMERA_H
