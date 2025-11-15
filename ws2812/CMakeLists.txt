cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(led_matrix C CXX ASM)

pico_sdk_init()

add_executable(led_matrix
    ws2812.c
    ws2812.pio
)

pico_generate_pio_header(led_matrix ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

target_link_libraries(led_matrix pico_stdlib hardware_pio)
pico_enable_stdio_usb(led_matrix 1)

pico_add_extra_outputs(led_matrix)

