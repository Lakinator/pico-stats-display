cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(temp_display_project)

pico_sdk_init()

add_subdirectory(lib)

add_executable(temp_display
  main.c
)

# This will enable serial output via USB
pico_enable_stdio_usb(temp_display 1)

pico_add_extra_outputs(temp_display)

target_link_libraries(temp_display epd_1in54_v2 epd_gui_paint epd_fonts bme280_spi)