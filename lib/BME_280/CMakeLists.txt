add_library(bme280_spi bme280_spi.c)
target_link_libraries(bme280_spi pico_stdlib hardware_spi)