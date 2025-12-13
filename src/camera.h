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

    bool startCamera(QLabel *targetLabel, int width = 640, int height = 480, int framerate = 30);
    void stopCamera();

    bool isCameraRunning() const;

    // Get current frame (as QImage)
    QImage get_frame() const;

signals:
    void frameCaptured(const QImage &image);   // emitted each frame

private slots:
    void captureFrame();

private:
    QTimer *timer;
    QLabel *displayLabel;

    cv::VideoCapture cap;
    bool running;

    int camWidth;
    int camHeight;
    int camFps;

    QImage lastFrame;  // store latest frame
};

#endif // CAMERA_H
