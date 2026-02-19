/**
 *  MIT License
 *  Copyright (c) E.Schilling
 *  See accompanying LICENSE file
 */

#include "mainwindow.h"
#include "telegram.h"

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
using namespace std;
#include <QApplication>

// define telegram bot instance for usage in mainwindow and camera
Telegram telegramBot;

int main(int argc, char *argv[])
{   
    // application gui setup
    QApplication a(argc, argv);
    MainWindow w; //kivi_bot_token, chat_id_leo
    w.show();
    return a.exec();

    return 0;
   
}
