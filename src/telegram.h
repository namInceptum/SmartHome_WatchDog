/**
 *  MIT License
 *  Copyright (c) E.Schilling
 *  See accompanying LICENSE file
 */


#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <tgbot/tgbot.h>
#include <string>
#include <memory>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "camera.h"

// for mapping of bot buttons and button-actions
#include <map>
#include <functional>

class Telegram {
public:
    // Constructor to initialize the Telegram bot with the given token
    Telegram(); //const std::string& token

    // Send a text message to a specific chat_id
    void send_txt_msg(const std::string& message); //int64_t chat_id, 

    // Send a photo to a specific chat_id
    void send_photo(const std::string& photo_path, const std::string& photo_mime_type); //int64_t chat_id,

    // send an frame object
    void send_frame(QImage frame);

    // Send a video to a specific chat_id
    void send_video( const std::string& video_path); // int64_t chat_id,
    // start the telegram bot and listen for commands
    void start();
    void stop();

    // set camera for sending images
    void setCamera(Camera *cam);
    void setChatID(int64_t newChatID);

public:
    // The Telegram bot instance, wrapped in a shared pointer
    std::shared_ptr<TgBot::Bot> bot_;
     // define bot config
    std::string ground_path = "/data/manual.jpg";
    std::string photo_mime_type = "image/jpeg";
    std::string bot_token = "replaceMeByYourBotToken";
    int64_t chat_id = 12345678; // replace by your chat ID

private:
    /*init camera with nulltpr*/
    Camera *bot_cam = nullptr;
    // define internal functions
    void handleMessage(TgBot::Message::Ptr message);
    void handleCallback(TgBot::CallbackQuery::Ptr query);
    bool save_document(TgBot::Message::Ptr msg);
    void showHelpOptions(int64_t chatId);
    
    
    // 
    std::map<std::string, std::function<void(int64_t)>> actionMap_;
    void initActionMap();

    // Action functions
    void handleSendImg(int64_t chatId);
    void handleRotCam180(int64_t chatId);


};

// define an instance of the telegram bot for usage in all .cpp files
extern Telegram telegramBot;

#endif // TELEGRAM_H
