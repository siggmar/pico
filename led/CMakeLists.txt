cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Name des Projekts

# Pico SDK einbinden
include(~/dev/pico/pico-sdk/external/pico_sdk_import.cmake)

project(led_project)

# Projekt initialisieren
pico_sdk_init()

# Executable erstellen
add_executable(${PROJECT_NAME}
    led.c
)

# Pico Standardlibs anhängen
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_pwm)

pico_add_extra_outputs(${PROJECT_NAME})


