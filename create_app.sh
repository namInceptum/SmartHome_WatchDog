
#delete old build path
rm -rf build

# build the tool
cmake -B build -S . -DCMAKE_PREFIX_PATH=/usr/lib/qt6

# create build components and compile
cmake --build build
# copy model into the application path
cp models/yolov8n.onnx build

# start application
sudo ./build/SmarthomeWatchdog
