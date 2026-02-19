
cmake_minimum_required(VERSION 3.19)

project(SmarthomeWatchdog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to ONNX Runtime
set(ONNXRUNTIME_DIR "/usr/local/onnxruntime")

# Ensure correct Qt version and modules 
set(CMAKE_PREFIX_PATH "/usr/lib/qt6")
find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui Multimedia MultimediaWidgets Widgets)

# Standard Qt setup: includes C++ standard, warnings, etc.
qt_standard_project_setup()

# Find OpenCV (if not found, install using: sudo apt-get install libopencv-dev)
find_package(OpenCV REQUIRED)

# Define the executable and UI/Source files
qt_add_executable(SmarthomeWatchdog
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/telegram.cpp
    src/telegram.h
    src/camera.cpp
    src/camera.h   
    src/yolo_engine.cpp
    src/yolo_engine.hpp
    src/yolo.cpp
    src/yolo.hpp
   
    ${UI_FILES}
    
)

# Telegram bot related imports
add_subdirectory(libs/tgbot-cpp)  # This builds the tgbot library

# Add UI files
set(UI_FILES
    mainwindow.ui
)

# Link to the directory containing the static library
link_directories(libs/tgbot-cpp/build)  # Ensure linker can find the static lib

# Link necessary Qt libraries and tgbot
target_link_libraries(SmarthomeWatchdog
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui 
        Qt6::Multimedia 
        Qt6::MultimediaWidgets
        TgBot  # Link the tgbot library, in the build folder of the library the file name is libTgBot.a
	${OpenCV_LIBS}
    onnxruntime

)

## onnxruntime includes
target_include_directories(SmarthomeWatchdog PRIVATE
    ${ONNXRUNTIME_DIR}/include)
# Link ONNX Runtime library directory
target_link_directories(SmarthomeWatchdog PRIVATE
    ${ONNXRUNTIME_DIR}/lib)
## install the yolo model



# Install rules
include(GNUInstallDirs)

install(TARGETS SmarthomeWatchdog
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

