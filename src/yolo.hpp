#ifndef YOLO_HPP
#define YOLO_HPP

#include "yolo_engine.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
// ----------------------------------------------------
// Global YOLO settings & objects (declared here)
// ----------------------------------------------------

// Path to YOLO model
extern std::string modelPath;

// ONNX Runtime logging ID
extern const char* logid;

// Provider (CPU / CUDA etc.)-> on raspi cpu
extern const char* provider;

// Class IDs of objects we want to detect
extern std::vector<int> obj_ids;

// Global YOLO inferencer
extern YoloInferencer inferencer;

// ----------------------------------------------------
// Function declarations
// ----------------------------------------------------

// generate color for the image labeling of detected objects
cv::Scalar generateRandomColor(int numChannels);
std::vector<cv::Scalar> generateRandomColors(int class_names_num, int numChannels);

// Perform detection → returns (object_detected, processed_image)
std::pair<bool, cv::Mat> detect(cv::Mat image);

#endif // YOLO_HPP
