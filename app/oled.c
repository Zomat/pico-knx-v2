#include "oled.h"

SSD1306 oled = {};

void oled_init()
{
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA);
    gpio_pull_up(OLED_SCL);

    SSD1306_init(&oled, i2c1);
}

void oled_show_addresses(DisplayAddress addresses[], size_t size)
{
    SSD1306_ClearDisplay(&oled);
    for (uint8_t i = 0; i < size; i++) {
        SSD1306_WriteText(&oled, strcat(addresses[i].name, ": "));
        SSD1306_NextLine(&oled);
        SSD1306_WriteText(&oled, addresses[i].address);
        SSD1306_NextLine(&oled);
        SSD1306_NextLine(&oled);
    }
}

void oled_show_splash_screen()
{
    SSD1306_font *temp_font = oled.fontPtr;

    SSD1306_ClearDisplay(&oled);
    SSD1306_SetFont(&oled, "8x16");
    SSD1306_WriteText(&oled, "Pico KNX v2");
    SSD1306_NextLine(&oled);
    SSD1306_NextLine(&oled);
    SSD1306_SetFont(&oled, "5x8");
    SSD1306_WriteText(&oled, "BY MATEUSZ ZOLISZ 2024");
    
    oled.fontPtr = temp_font;
}