#!/bin/sh

# to use the dedicated Nvidia GPU instead of the Intel's integrated GPU, use the following two lines
export __NV_PRIME_RENDER_OFFLOAD=1 # offload rendering tasks for the subsequent application to the NVIDIA discrete GPU.
export __GLX_VENDOR_LIBRARY_NAME=nvidia # instruct libglvnd to load and use the OpenGL implementation provided by the nvidia driver


rm -rf build
rm -rf bin
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)
cd ..
./bin/GAME_APPLICATION