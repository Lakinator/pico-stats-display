#ifndef __BME_280_SPI__
#define __BME_280_SPI__

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define BME_PIN_MISO 12
#define BME_PIN_CS 9
#define BME_PIN_SCK 10
#define BME_PIN_MOSI 11

#define BME_SPI_PORT spi1
#define READ_BIT 0x80

void bme280_init();
void bme280_read_raw(int32_t *humidity, int32_t *pressure, int32_t *temperature);
int32_t compensate_temp(int32_t adc_T);
uint32_t compensate_pressure(int32_t adc_P);
uint32_t compensate_humidity(int32_t adc_H);

#endif