#include "yolo_engine.hpp"
#include "yolo.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random> // for random color generation
#include <filesystem>

// to get the current path of the executable for loading the yolo model
namespace fs = std::filesystem;

// define constants
std::string modelPath = fs::current_path().string() + "/yolov8n.onnx";
const char* logid = "yolo_inference";
const char* provider = "CPU";
std::vector<int> obj_ids {0, 1, 2, 3, 5, 16}; // we want to detect human = 0, bicycle = 1, car = 2, motorcycle 3, bus = 5, dog = 16,
YoloInferencer inferencer(modelPath, logid, provider);

// Define the skeleton and color mappings
cv::Scalar generateRandomColor(int numChannels) {
    if (numChannels < 1 || numChannels > 3) {
        throw std::invalid_argument("Invalid number of channels. Must be between 1 and 3.");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    cv::Scalar color;
    for (int i = 0; i < numChannels; i++) {
        color[i] = dis(gen); // for each channel separately generate value
    }

    return color;
}

std::vector<cv::Scalar> generateRandomColors(int class_names_num, int numChannels) {
    std::vector<cv::Scalar> colors;
    for (int i = 0; i < class_names_num; i++) {
        cv::Scalar color = generateRandomColor(numChannels);
        colors.push_back(color);
    }
    return colors;
}



std::pair<bool, cv::Mat> detect(cv::Mat image) {

    if (image.empty()) {
        throw std::runtime_error("Error: Unable to load image");
    }

    bool obj_detected = false;

    std::vector<Detection> detections = inferencer.infer(image, 0.1, 0.5); // get result of object detection 
    
    for (const auto& detection : detections) {
        // skip if the detection confidence is < 75%
        if(detection.confidence < 0.75){ // 
            continue;
        }
        // draw a rectangle around the detected object
        //cv::rectangle(image, detection.box, cv::Scalar(0, 255, 0), 2);
        cv::Scalar color = generateRandomColor(3);
        cv::rectangle(image, detection.box, generateRandomColor(3), 2); // draaw a box around the detected object
        std::string clean_name = detection.class_name; // get the name of the detected object, it is stored with ' ' in onnx
        clean_name.erase(std::remove(clean_name.begin(), clean_name.end(), '\''), clean_name.end()); // remove the single quotes ''

        std::string label =  clean_name + ":" + cv::format("%.2f", detection.confidence); // lable for the image 
        
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline); // .5 = fontscale, 1 = thicknes

        // background rectangle coordinates
        int x = detection.box.x;
        int y = std::max(0, detection.box.y - textSize.height - 4);

        // draw filled background rectangle
        cv::rectangle(image, cv::Rect(x, y, textSize.width + 4, textSize.height + 4), color, cv::FILLED );

        // draw text on top of the rectangle
        cv::Scalar text_color(255, 255, 255);
        cv::putText(image,label, cv::Point(x + 1, y + textSize.height + 1), cv::FONT_HERSHEY_SIMPLEX, 0.5, text_color,1);

        // check if the detected object is in the list
        if (std::find(obj_ids.begin(), obj_ids.end(), detection.class_id) != obj_ids.end()) {
            obj_detected = true;
        }
    }

    return {obj_detected, image};
}