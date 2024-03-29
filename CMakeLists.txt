cmake_minimum_required(VERSION 3.13)

set(PROGRAM_NAME pico_knx2)
set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)

project(pico_knx2)

pico_sdk_init()

message("Running makefsdata python script")
execute_process(COMMAND
    perl makefsdata
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

FILE(GLOB drivers_executables drivers/*.c)
FILE(GLOB app_executables app/*.c)
FILE(GLOB oled_images oledImages/*.c)

add_executable(${PROGRAM_NAME}
    main.c
    ${drivers_executables}
    ${app_executables}
    ${oled_images}
)

target_include_directories(${PROGRAM_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/drivers
)

target_link_libraries(${PROGRAM_NAME}
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    pico_stdlib
    hardware_adc
    hardware_i2c
    hardware_timer
    hardware_gpio
)

pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_enable_stdio_uart(${PROGRAM_NAME} FALSE)

pico_add_extra_outputs(${PROGRAM_NAME})