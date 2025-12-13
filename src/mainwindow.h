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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr); //const std::string& token, int64_t chatId,
    ~MainWindow();

    // private variables
private: 
    Ui::MainWindow *ui;
    Camera *camera;


private:
    QPalette comboBoxPalette;
    
    QMediaCaptureSession *captureSession;
    QVideoWidget *videoWidget;

    // general setup
    void setupSettings();

    // if the camera fails to start initially try to restart
    void on_send_frame_btn();

    // control frame orientation, here flipping option only vertically
    void on_cancel_app_btn();

    // 
    void onFrameCaptured(const QImage &img);

};

#endif // MAINWINDOW_H
