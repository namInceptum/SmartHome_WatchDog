// include local headder files
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camera.h"
#include "telegram.h"

#include <QCameraDevice>
#include <QMediaDevices>  // Needed to list available cameras

#include <QPushButton>
#include <QFile>

// libraries for taking screenshot
/*
#include <QScreen>
#include <QDateTime>
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>
#include <QDir>
#include <QFile> */

#include <iostream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) //, telegramBot(token), chat_id_leo(chatId)
{
    ui->setupUi(this);

    // start camera
    camera = new Camera(this);
    camera->startCamera(ui->videoLabel);

    //connect(camera, &Camera::frameCaptured,this, &MainWindow::onFrameCaptured);
    // Set the default settings
    this->setupSettings();


    
    // send image if the button send_frame is clicked -> just for fun
    connect(ui->send_frame_btn, &QPushButton::clicked, this, &MainWindow::on_send_frame_btn);
    // exit appkication
    connect(ui->cancel_app_btn, &QPushButton::clicked, this, &MainWindow::on_cancel_app_btn);

}

MainWindow::~MainWindow()
{
    //delete camera;
    delete ui;
}


void MainWindow::setupSettings()
{
    // Set items needed for QSettings  -> real information not needed here
    QCoreApplication::setOrganizationName("BOMKE");
    QCoreApplication::setOrganizationDomain("None");
    QCoreApplication::setApplicationName("SmartHome WatchDog");
    // Create the object to access settings
    QSettings settings;
}


void MainWindow::on_send_frame_btn(){

    QImage frame = camera->get_frame();
    QString filePath = "/tmp/manual.jpg";

    // Check if the file already exists
    QFile file(filePath);
    if (file.exists()) {
        // Delete the existing file
        file.remove();
    }
    frame.save(filePath);

    telegramBot.send_photo("/tmp/manual.jpg", "image/jpeg");

}

void MainWindow::on_cancel_app_btn(){

    qDebug() << "on cancel app clicked";
    telegramBot.send_txt_msg("on cancel app clicked");

    qDebug() << "Exit button clicked";
    int ret = QMessageBox::question(this, 
        "Confirm Exit", 
        "Are you sure you want to exit the application?",
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        qDebug() << "User confirmed exit";
        QApplication::quit();  // Exits the application
    }

}

void MainWindow::onFrameCaptured(const QImage &img)
{
    /*
    QString tempPath = "/tmp/captured.jpg";
    img.save(tempPath, "JPG");

    telegramBot.send_photo(tempPath.toStdString(),"image/jpeg");

    qDebug() << "Photo sent to Telegram!"; */
}


