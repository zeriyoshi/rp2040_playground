cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME rp2040_playground)

set(PICO_BOARD pico)
include(pico_sdk_import.cmake)
project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    pico_multicore
    hardware_gpio
    hardware_pwm
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.uf2
    DESTINATION .
)
