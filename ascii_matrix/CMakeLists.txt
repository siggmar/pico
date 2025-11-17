cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(ascii_art C CXX ASM)

pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.c
    board.pio
)

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/board.pio)

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_pio)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

