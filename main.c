#include <stdio.h>
#include <stdlib.h>
#include "lib/GUI/GUI_Paint.h"
#include "lib/EPD_1in54_V2/EPD_1in54_V2.h"
#include "lib/BME_280/bme280_spi.h"

// convert bmp to byte array: https://www.waveshare.com/wiki/File:Image2Lcd.7z

#define PIN_LED 25
#define PIN_TOUCH 14

#define TEXT_BUF_SIZE 16
#define REFRESH_SLEEP_SEC 60
#define REFRESH_SEGMENTS 4

void blink(UBYTE amount, UDOUBLE time)
{
    UBYTE i = 0;
    while (i < amount)
    {
        sleep_ms(time);
        gpio_put(PIN_LED, 1);
        sleep_ms(time);
        gpio_put(PIN_LED, 0);
        i++;
    }
}

int main()
{
    stdio_init_all();
    gpio_init(PIN_LED);
    gpio_init(PIN_TOUCH);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_set_dir(PIN_TOUCH, GPIO_IN);

    blink(2, 150);

    // Init EPD //
    Init_Device();
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();

    // Create new image cache
    UBYTE *ImageBuffer;
    UWORD Imagesize = ((EPD_1IN54_V2_WIDTH % 8 == 0) ? (EPD_1IN54_V2_WIDTH / 8) : (EPD_1IN54_V2_WIDTH / 8 + 1)) * EPD_1IN54_V2_HEIGHT;
    if ((ImageBuffer = (UBYTE *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for black memory...\r\n");
    }
    Paint_NewImage(ImageBuffer, EPD_1IN54_V2_WIDTH, EPD_1IN54_V2_HEIGHT, 270, WHITE);
    Paint_SelectImage(ImageBuffer);

    // Init BME //
    bme280_init();
    int32_t humidity, pressure, temperature;
    char hString[TEXT_BUF_SIZE + 1];
    hString[TEXT_BUF_SIZE] = '\0';
    char pString[TEXT_BUF_SIZE + 1];
    pString[TEXT_BUF_SIZE] = '\0';
    char tString[TEXT_BUF_SIZE + 1];
    tString[TEXT_BUF_SIZE] = '\0';

    blink(1, 250);

    while (1)
    {
        // Read BME values //

        bme280_read_raw(&humidity, &pressure, &temperature);

        pressure = compensate_pressure(pressure);
        temperature = compensate_temp(temperature);
        humidity = compensate_humidity(humidity);

        snprintf(hString, TEXT_BUF_SIZE, "%.2f%%", humidity / 1024.0);
        snprintf(pString, TEXT_BUF_SIZE, "%.2fBar", pressure / 100000.0); // Pascal = pressure, Bar = pressure / 100000.0
        snprintf(tString, TEXT_BUF_SIZE, "%.2fC", temperature / 100.0);

        printf("%s\n", hString);
        printf("%s\n", pString);
        printf("%s\n", tString);

        // Display values on EPD //

        EPD_1IN54_V2_Init(); // Wake from sleep

        Paint_Clear(WHITE);

        Paint_DrawString_EN(50, 5, tString, &Font24, WHITE, BLACK); // Temperature

        Paint_DrawString_EN(30, 5 + (2 * Font24.Height), "-Humidity-", &Font20, WHITE, BLACK);            // Humidity
        Paint_DrawString_EN(55, 5 + (2 * Font24.Height) + Font20.Height, hString, &Font20, WHITE, BLACK); // Humidity value

        Paint_DrawString_EN(30, 5 + (2 * Font24.Height) + (3 * Font20.Height), "-Pressure-", &Font20, WHITE, BLACK); // Pressure
        Paint_DrawString_EN(50, 5 + (2 * Font24.Height) + (4 * Font20.Height), pString, &Font20, WHITE, BLACK);      // Pressure value

        Paint_DrawRectangle(1, 180, 199, 200, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); // Timer bar

        EPD_1IN54_V2_Display(ImageBuffer);

        // Timer countdown //

        EPD_1IN54_V2_DisplayPartBaseImage(ImageBuffer);

        for (int i = 1; i <= REFRESH_SLEEP_SEC; i++)
        {
            if (i % (REFRESH_SLEEP_SEC / REFRESH_SEGMENTS) == 0) {
                Paint_ClearWindows(0, 180, 200, 200, WHITE);
                Paint_DrawRectangle(0, 180, (int) (200 / ((double) REFRESH_SLEEP_SEC / i)), 200, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawRectangle(1, 180, 199, 200, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
                EPD_1IN54_V2_DisplayPart(ImageBuffer);
                EPD_1IN54_V2_Sleep();
            }

            int touch = gpio_get(PIN_TOUCH);
            if (touch == 1) break;

            sleep_ms(1000);
        }
    }

    // EXIT //
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();
    EPD_1IN54_V2_Sleep();

    free(ImageBuffer);

    sleep_ms(2000);
    blink(2, 150);
}