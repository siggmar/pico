cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(oled_example C CXX ASM)
pico_sdk_init()

add_executable(oled_example
        screen.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(oled_example pico_stdlib hardware_i2c)


pico_enable_stdio_usb(oled_example 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(oled_example)

