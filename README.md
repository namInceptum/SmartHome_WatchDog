
# Custom Home Surveillance System with Raspberry Pi, Telegram API, and YOLO Object Detection

This project aims to develop a private, customizable home surveillance system powered by a Raspberry Pi, integrated with the Telegram API and utilizing the YOLOv8 object detection model.

The system continuously records the environment using a webcam. In the background, the YOLOv8 model processes the video feed in real-time to detect various objects such as people, pets (dogs), vehicles (cars, motorcycles, bikes), and other potential security threats.

When suspicious objects are detected, the system automatically sends images to your smartphone via Telegram, ensuring that you're always informed of any unusual activity in your home environment. The system is highly customizable, allowing you to adjust the types of objects you want to monitor based on your specific needs.


### Video Tutorial:
On my channel [BOMKE](https://https://youtu.be/02UeHkQm1bw)

## Prerequisites

Before we start, ensure you have the following installed on your RaspberryPI:

* Qt 6 

* Visual Studio Code

* Qt Extension Pack for VS Code

* CMake

* A C++ Compiler (such as MinGW or MSVC)

### Step 1: Download this Repo
Open the Terminal in your project folder and run:
```bash
git clone ... 
```

### Step 2: Open VS Code.
In the current Terminal open VS Code:
```bash
code .
``` 

### Step 3. Create a Telegram Bot
To send images to your smartphone you need to configure a Telegram bot:
- Open **Telegram Web** in your browser and log in.
- Search for **BotFather** and start a chat with BotFather.
- Select `/newbot`.
- Enter a name for your bot (e.g., `BOMKE`).
- Enter a username for your bot (e.g., `BOMKE_bot`).
- Copy and save the **bot token** shown in the BotFather message.
- Search for your bot, start a chat, and message it.
- Open a new tab and go to [https://api.telegram.org/botYourBotToken/getUpdates](https://api.telegram.org/bot<YourBotToken>/getUpdates). To get the ChatID replace in the url 'YourBotToken' wiht your token from the BotFather.
- This will show you a `.json` file containing some information. If it's empty, message your bot again in Telegram Web and reload the page.
- On this page, search for the bot name and find the **chat ID**. We need the chat ID and the bot token later.

### Step 4. Open Project
Navigate to the `telegram.h` file of the cloned project and enter the bot token and chat ID in the following variables:
```cpp
bot_token = "skdjflJL35432"
chat_id = 12345678
```

### Step 5. WebCam
Connect the USB-WebCam to the Raspi.


### Step 6: Run The Project
We have created a shell script, `create_app.sh`, which builds the project. Therefore, we need to change the mode of this file to make it executable:

```bash
chmod +x create_app.sh
```
Now you can run build and run the project:
```bash
./create_app.sh
```
The binary file of the project is in the /build/SmartHomeWatchDog folder.
A small user interface will open, showing the current video stream. You can manually send images to your Telegram or exit the app.

Once you're in your Telegram chat on your smartphone, message the bot. If you send the command `opts`, the bot will respond with the currently implemented options as interactive buttons.

Currently, you can:

- SendImage: Click this button, and the bot will send you an image from the current video stream.

- Rotate Camera: Click this button to rotate the camera by 180 degrees.

You can easily implement additional functionalities by following the pattern of these two button implementations.
**Have fun!** 😊


### Example Images
The small user interface shows the current video Stream.

![User Interface](/media/ui.jpg)

If objects are detected you will get directly msgs on your telegram app.

![Sending of detected objects to telegram](/media/detect.jpg)


