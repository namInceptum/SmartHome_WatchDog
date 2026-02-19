////////////////////////////////////
// provide telegram bot functionality
// e.g. sending of images or videos
////////////////////////////////////
/**
 *  MIT License
 *  Copyright (c) E.Schilling
 *  See accompanying LICENSE file
 */

 

#include "telegram.h"
#include "camera.h"

#include <iostream>
#include <QThread>
#include <QFile>
#include <atomic>
#include <chrono>
#include <thread>

//// telegram thread
#include "telegram.h"
#include <QThread>
#include <atomic>
#include <chrono>
#include <thread>

#include <ctime>
#include <iomanip>
#include <sstream>


namespace {

// Internal telegram_thread thread -> handling of incoming and received messages in a parallel thread
// to the camera object detection
class TelegramThread : public QThread {
public:
    TelegramThread(std::shared_ptr<TgBot::Bot> bot)
        : bot_(bot), running_(true) {}

    void stop() {
        running_ = false;
        requestInterruption();
    }

protected:
    void run() override {
        TgBot::TgLongPoll longPoll(*bot_);

        while (running_ && !isInterruptionRequested()) { // continiously pooling messages 
            try {
                longPoll.start();
            }
            catch (const std::exception& e) {
                std::cerr << "Long poll error: "
                          << e.what() << std::endl;

                // Prevent tight CPU loop on network failure
                std::this_thread::sleep_for(
                    std::chrono::seconds(2));
            }
        }
    }

private:
    std::shared_ptr<TgBot::Bot> bot_;
    std::atomic<bool> running_;
};

} // anonymous namespace
// create telegram thread for receiving and sending messages
static TelegramThread* telegram_thread = nullptr;



Telegram::Telegram() {
    // create bot instance to use for interaction
    bot_ = std::make_shared<TgBot::Bot>(bot_token);

    // delete webhook in polling mode
    try{
        bot_->getApi().deleteWebhook();
    }catch (const std::exception& e){
        std::cerr << "Error while deleting webhook in pooling mode" << std::endl;
    }

     // listen to incoming text messages
    bot_->getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
        handleMessage(message);
    });
        // Button / inline keyboard callbacks
    bot_->getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
        handleCallback(query);
    }); 

    // initialize action map for bot
    this->initActionMap();

    // start bot
    this->start();

}

// Send a text message to a specific chat_id
void Telegram::send_txt_msg( const std::string& message) {
    try {
        bot_->getApi().sendMessage(chat_id, message);
        std::cout << "Message sent: " << message << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error sending message: " << e.what() << std::endl;
    }
}

// Send a photo to a specific chat_id
void Telegram::send_photo(const std::string& photo_path, const std::string& photo_mime_type) {
    try {
        bot_->getApi().sendPhoto(chat_id, TgBot::InputFile::fromFile(photo_path, photo_mime_type));
        std::cout << "Photo sent: " << photo_path << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Error sending photo: " << e.what() << std::endl;
    }
}

// send given frame-> first save it then send it
void Telegram::send_frame(QImage frame){
    try{
        
        QFile file(QString::fromStdString(ground_path));
        if (file.exists()) {
            // Delete the existing file
            file.remove();
            qDebug() << "removing previous image";
        }
        
        frame.save(QString::fromStdString(ground_path));

        telegramBot.send_photo(ground_path, "image/jpeg"); //"/tmp/manual.jpg"
    }catch (const TgBot::TgException& e){
        std::cerr << "ERROR: could not send frame using telegram bot: " << e.what() << std::endl;
    
    }catch(const std::exception& e){
        std::cerr << "Std Error: " << e.what() << std::endl;

    }
    
    
    
}


// Start the bot and listen for events (like /start or /photo commands)
void Telegram::start() {

    if (!telegram_thread) {
        telegram_thread = new TelegramThread(bot_);
        telegram_thread->start();
    }
}

void Telegram::stop()
{
    if (telegram_thread) {
        telegram_thread->stop();
        telegram_thread->wait();
        delete telegram_thread;
        telegram_thread = nullptr;
    }
}


// handle incoming msgs from the user
void Telegram::handleMessage(TgBot::Message::Ptr msg) {
    
    // get chat id of the message
    const int64_t msg_chat_id = msg->chat->id;

    std::cout << "Configured chat_id: " << chat_id
              << " Incoming chat_id: "
              << msg_chat_id << std::endl;
    // ignore unknown users
    if (msg_chat_id != chat_id){
        
        bot_->getApi().deleteMessage(msg_chat_id, msg->messageId); // delete message of the user and ignore 
        return;
    }
    // Ignore groups
    if (msg->chat->type != TgBot::Chat::Type::Private){
        return;
    }
    
    // DOCUMENT MESSAGE
    if(msg->document){
        std::cout << "Got document, try to save ... " << std::endl;
        bool doc_saved = this->save_document(msg);
        if(doc_saved){
            bot_->getApi().sendMessage(msg_chat_id, "document saved");
        }
        bot_->getApi().sendMessage(msg_chat_id, "document download failed");
    }


    // receive TEXT MESSAGE
    if (!msg->text.empty()) {

        std::string text = msg->text;
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);
        std::cout << "got message: " << text << std::endl;

        if (text == "opts") {
            // show options
            this->showHelpOptions(msg_chat_id);
            qDebug() << "show user options";
            return;
        }

        if (text == "help") {
            bot_->getApi().sendMessage(msg_chat_id, "Type 'opts' to see available options.");
            return;
        }

        if (text.rfind("del vid:", 0) == 0) {
            std::string path = text.substr(8);
            // delete video logic
            return;
        }

        bot_->getApi().sendMessage(msg_chat_id,
            "The message \"" + text + "\" could not be processed.\nType 'help' or 'opts'.");
        return;
    }

    

    
}


// show help options to the user
void Telegram::showHelpOptions(int64_t chatId) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard =
    std::make_shared<TgBot::InlineKeyboardMarkup>();

    // Helper lambda to create buttons
    auto makeButton = [](const std::string& text,
                         const std::string& callback) {
        TgBot::InlineKeyboardButton::Ptr btn =
            std::make_shared<TgBot::InlineKeyboardButton>();
        btn->text = text;
        btn->callbackData = callback;
        return btn;
    };

    // Create buttons
    auto btnRotCam180      = makeButton("RotCam180",     "RotCam180");
    auto btnSendImage         = makeButton("SendImage",     "SendImage");

    // Add buttons in rows of 3 (matrix layout)
    keyboard->inlineKeyboard = {
        { btnRotCam180,  btnSendImage }
    };

    try {
        bot_->getApi().sendMessage(
            chatId,
            "Select an option:",
            nullptr,
            0,
            keyboard
        );
    }
    catch (const std::exception& e) {
        std::cerr << "Error sending keyboard: "
                  << e.what() << std::endl;
    }

    
}


// handle help-option buttons if one of them is pressed:
void Telegram::handleCallback(TgBot::CallbackQuery::Ptr query) {

    if (!query || !query->message || !query->message->chat)
        return;

    const std::string& data = query->data;
    const int64_t chatId = query->message->chat->id;
    std::cout << "Button clicked: " << data << std::endl;
    auto it = actionMap_.find(data);

    if (it != actionMap_.end()) {
        it->second(chatId);  // Call mapped function
    } else {
        bot_->getApi().sendMessage(chatId, "Unknown option.");
    }

    try {
        // Always answer callback to stop Telegram loading spinner
        bot_->getApi().answerCallbackQuery(query->id);

        //dispatchCallback(data, chatId, query);

    } catch (const std::exception& e) {
        std::cerr << "Callback error: "
                  << e.what() << std::endl;
    }
   
}



// initialize action map
void Telegram::initActionMap(){
    actionMap_["SendImage"]        = [this](int64_t id){ handleSendImg(id); };
    actionMap_["RotCam180"]      = [this](int64_t id){ handleRotCam180(id); };
   
}

//////////////////////// Begin button-action buttons /////////////

void Telegram::handleSendImg(int64_t chatId){
    if (!bot_cam || !bot_cam->isCameraRunning()) {
        bot_->getApi().sendMessage(chatId, "Camera not running.");
        return;
    }

    qDebug() << "Sending image...";

    try {
        send_frame(bot_cam->get_frame());
    }
    catch (const std::exception& e) {
        bot_->getApi().sendMessage(chatId,
            std::string("Failed to send image: ") + e.what());
    }
}



void Telegram::handleRotCam180(int64_t chatId){
    qDebug() << "Rotating camera 180 degrees...";

    if (bot_cam) {
        if(bot_cam->getRotate180()){ // if the cam is rotated
            bot_cam->setRotate180(false); // then rotate it back
            }else{
                bot_cam->setRotate180(true); // or rotate again
                }
        bot_->getApi().sendMessage(chatId, "Camera rotated 180°.");
    } else {
        bot_->getApi().sendMessage(chatId, "Camera not available.");
    }
}


// handle document
bool Telegram::save_document(TgBot::Message::Ptr msg){
    if (!msg->document)
        return false;

    try {
        std::string fileId = msg->document->fileId;
        std::string fileName = msg->document->fileName;

        // --- Fallback to timestamp if filename is empty ---
        if (fileName.empty()) {
            std::time_t t = std::time(nullptr);
            std::tm tm{};
            localtime_r(&t, &tm); // thread-safe version

            std::ostringstream oss;
            oss << std::put_time(&tm, "%y-%m-%d-%H-%M-%S"); // yy-mm-dd-HH-MM-SS
            fileName = oss.str();
        }

        // Get Telegram file path
        TgBot::File::Ptr file = bot_->getApi().getFile(fileId);

        // Open local file for writing
        std::ofstream out(fileName, std::ios::binary);
        if (!out.is_open()) {
            std::cerr << "Cannot open file: " << fileName << std::endl;
            return false;
        }

        // Download file content
        std::string content = bot_->getApi().downloadFile(file->filePath);
        out.write(content.c_str(), content.size());
        out.close();

        std::cout << "Saved file: " << fileName << std::endl;
        return true;
    }
    catch (const std::exception &e) {
        std::cerr << "Error saving document: " << e.what() << std::endl;
        return false;
    }

}


//////  set camera
void Telegram::setCamera(Camera *cam)
{
    this->bot_cam = cam;
}

void Telegram::setChatID(int64_t newChatID){
    this->chat_id = newChatID;
    return;
    }

