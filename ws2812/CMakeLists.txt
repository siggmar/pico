cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(~/dev/pico/pico-sdk/external/pico_sdk_import.cmake)

project(ws2812_example)

pico_sdk_init()

add_executable(ws2812
    ws2812.c
    ws2812.pio
)

pico_generate_pio_header(ws2812 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

target_link_libraries(ws2812 pico_stdlib hardware_pio)
pico_enable_stdio_usb(ws2812 1)

pico_add_extra_outputs(ws2812)

