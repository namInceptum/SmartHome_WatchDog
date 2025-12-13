////////////////////////////////////
// provide telegram bot functionality
// e.g. sending of images or videos
////////////////////////////////////

#include "telegram.h"
#include <iostream>
#include "camera.h"




Telegram::Telegram() {
    bot_ = std::make_shared<TgBot::Bot>(kivi_bot_token);
}

// Send a text message to a specific chat_id
void Telegram::send_txt_msg( const std::string& message) {
    try {
        bot_->getApi().sendMessage(chat_id_leo, message);
        std::cout << "Message sent: " << message << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error sending message: " << e.what() << std::endl;
    }
}

// Send a photo to a specific chat_id
void Telegram::send_photo(const std::string& photo_path, const std::string& photo_mime_type) {
    try {
        bot_->getApi().sendPhoto(chat_id_leo, TgBot::InputFile::fromFile(photo_path, photo_mime_type));
        std::cout << "Photo sent: " << photo_path << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error sending photo: " << e.what() << std::endl;
    }
}

// send given frame-> first save it then send it
void Telegram::send_frame(cv::Mat frame){
    try{
        cv::imwrite(ground_path, frame);
        bot_->getApi().sendPhoto(chat_id_leo, TgBot::InputFile::fromFile(ground_path, photo_mime_type));
    }catch (const TgBot::TgException& e){
        std::cerr << "ERROR: could not send frame using telegram bot: " << e.what() << std::endl;
    
    }catch(const std::exception& e){
        std::cerr << "Std Error: " << e.what() << std::endl;

    }
    
}

// Send a video to a specific chat_id
void Telegram::send_video(const std::string& video_path) {
    try {
        bot_->getApi().sendVideo(chat_id_leo, TgBot::InputFile::fromFile(video_path, "video/mp4"));
        std::cout << "Video sent: " << video_path << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error sending video: " << e.what() << std::endl;
    }
}

// Start the bot and listen for events (like /start or /photo commands)
void Telegram::start() {
    // Event handler for "/start" command
    bot_->getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
        bot_->getApi().sendMessage(message->chat->id, "Hi! Welcome to the Smart Home Assistant!");
    });

    // Event handler for "/photo" command
    /* --> not yet needed
    bot_->getEvents().onCommand("photo", [this](TgBot::Message::Ptr message) {
        const std::string photoFilePath = "example.jpg";
        const std::string photoMimeType = "image/jpeg";
        bot_->getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile(photoFilePath, photoMimeType));
    }); */

    // Start the long polling process to listen for events
    try {
        TgBot::TgLongPoll longPoll(*bot_);
        longPoll.start();
    } catch (const std::exception& e) {
        std::cerr << "Error starting bot: " << e.what() << std::endl;
    }
}
