#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <tgbot/tgbot.h>
#include <string>
#include <memory>
#include <iostream>
#include <opencv2/opencv.hpp>

class Telegram {
public:
    // Constructor to initialize the Telegram bot with the given token
    Telegram(); //const std::string& token

    // Send a text message to a specific chat_id
    void send_txt_msg(const std::string& message); //int64_t chat_id, 

    // Send a photo to a specific chat_id
    void send_photo(const std::string& photo_path, const std::string& photo_mime_type); //int64_t chat_id,

    // send an frame object
    void send_frame(cv::Mat frame);

    // Send a video to a specific chat_id
    void send_video( const std::string& video_path); // int64_t chat_id,
    // start the telegram bot and listen for commands
    void start();

private:
    // The Telegram bot instance, wrapped in a shared pointer
    std::shared_ptr<TgBot::Bot> bot_;
     // define bot config
    std::string ground_path = "/tmp/manual.jpg";
    std::string photo_mime_type = "image/jpeg";
    std::string kivi_bot_token = "73;
    int64_t chat_id_leo = 24; // only message one person
};

// define an instance of the telegram bot for usage in all .cpp files
extern Telegram telegramBot;

#endif // TELEGRAM_H
