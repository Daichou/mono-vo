set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-xilinx-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER arm-xilinx-linux-gnueabi-g++)
set(CMAKE_C_FLAG -mcpu=cortex-a9 -mfpu=neon-vfpv4 -ftree-vectorize -mfloat-abi=hard -O2)
set(CMAKE_CXX_FLAG -mcpu=cortex-a9 -mfpu=neon-vfpv4 -ftree-vectorize -mfloat-abi=hard -O2)
set(CMAKE_INSTALL_PREFIX "/home/ubuntu/opencv-3.1.0/install")
