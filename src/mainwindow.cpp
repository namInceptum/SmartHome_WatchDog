/**
 *  MIT License
 *  Copyright (c) E.Schilling
 *  See accompanying LICENSE file
 */


// include local headder files
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camera.h"
#include "telegram.h"

#include <QCameraDevice>
#include <QMediaDevices>  // Needed to list available cameras

#include <QPushButton>
#include <QFile>
#include <iostream>

// include standard library
#include <filesystem> // for walking through folders
#include <vector>
#include <string>
#include <iostream>
namespace fs = std::filesystem;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    // start camera
    camera = new Camera(this);
    camera->startCamera(ui->videoLabel);
    telegramBot.setCamera(camera);
    
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


void MainWindow::on_send_frame_btn(){

    QImage frame = camera->get_frame();

    QString filePath = "/data/manual.jpg";

    // Check if the file already exists
    QFile file(filePath);
    if (file.exists()) {
        // Delete the existing file
        file.remove();
    }
    frame.save(filePath);

    telegramBot.send_photo("/data/manual.jpg", "image/jpeg");

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



