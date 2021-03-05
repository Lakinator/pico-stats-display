#ifndef __EPD_1in54_V2__
#define __EPD_1in54_V2__

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "stdint.h"

/**
 * data
**/
#define UBYTE uint8_t
#define UWORD uint16_t
#define UDOUBLE uint32_t

/**
 * GPIO config
**/
#define EPD_PIN_DIN 3
#define EPD_PIN_CLK 2
#define EPD_PIN_CS 1
#define EPD_PIN_DC 15
#define EPD_PIN_RST 4
#define EPD_PIN_BUSY 7

#define EPD_SPI_PORT spi0

/**
 * Display resolution
**/
#define EPD_1IN54_V2_WIDTH 200
#define EPD_1IN54_V2_HEIGHT 200

void Init_Device();

void EPD_1IN54_V2_Reset();
void EPD_1IN54_V2_SendCommand(UBYTE Reg);
void EPD_1IN54_V2_SendData(UBYTE Data);
void EPD_1IN54_V2_ReadBusy();
void EPD_1IN54_V2_TurnOnDisplay();
void EPD_1IN54_V2_TurnOnDisplayPart();
void EPD_1IN54_V2_Init();
void EPD_1IN54_V2_Clear();
void EPD_1IN54_V2_Display(UBYTE *Image);
void EPD_1IN54_V2_DisplayPartBaseImage(UBYTE *Image);
void EPD_1IN54_V2_DisplayPart(UBYTE *Image);
void EPD_1IN54_V2_Sleep();

#endif