# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(BMI270 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# 主程序 (main 在 main.cpp, 驱动封装在 src/BMI270.cpp + include/BMI270.hpp)
add_executable(BMI270
    main.cpp
    src/BMI270.cpp
)

pico_set_program_name(BMI270 "BMI270")
pico_set_program_version(BMI270 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(BMI270 0)
pico_enable_stdio_usb(BMI270 1)

# 头文件路径
target_include_directories(BMI270 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Add the standard library to the build
target_link_libraries(BMI270
    pico_stdlib
    hardware_i2c
)

pico_add_extra_outputs(BMI270)
