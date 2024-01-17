/*
* SSD1306 Oled Display I2C Driver
* Creator: Mateusz Zolisz
* 2023
*/
#ifndef _SSD1306_I2C_H_
#define _SSD1306_I2C_H_

#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306_fonts.h"
#include "ssd1306_image.h"

#define SSD1306_HEIGHT                  64
#define SSD1306_WIDTH                   128

#define SSD1306_PAGE_HEIGHT             8
#define SSD1306_PAGES                   8

#define SSD1306_I2C_ADDR                0x3C
#define SSD1306_I2C_CLK                 400

#define SSD1306_SET_DISPLAY_ON          0xAF
#define SSD1306_SET_DISPLAY_OFF         0xAF
#define SSD1306_SET_DISPLAY_NORMAL      0xA6
#define SSD1306_SET_MEMORY_MODE         0x20
#define SSD1306_PAGE_ADDRESSING_MODE    0x02

#define SSD1306_CMD_REG                 0x80
#define SSD1306_DATA_REG                0x40

typedef struct {
    uint8_t column;
    uint8_t page;
    SSD1306_font* fontPtr;
    i2c_inst_t* i2cHandler;
} SSD1306;

/**
 * LOW LEVEL FUNCTIONS
 */
void SSD1306_WriteCmd(SSD1306* dev, uint8_t cmd);
void SSD1306_WriteData(SSD1306* dev, uint8_t data);

/**
 * INITALISATION
 */
void SSD1306_init();

/**
 * DISPLAY FUNCTIONS
 */
void SSD1306_ClearDisplay(SSD1306* dev);
void SSD1306_ClearPage(SSD1306* dev, uint8_t page);
void SSD1306_ClearSpace(SSD1306* dev, uint8_t x0, uint8_t x1, uint8_t page0, uint8_t page1);
void SSD1306_WriteText(SSD1306* dev, char *str);
void SSD1306_WriteImage(SSD1306* dev, SSD1306_image* image, uint8_t x, uint8_t y);
void SSD1306_SetCursor(SSD1306 *dev, uint8_t x, uint8_t y);
void SSD1306_NextLine(SSD1306 *dev);
void SSD1306_SetFont(SSD1306* dev, char *size);
void SSD1306_WritePixel(SSD1306 *dev, uint8_t x, uint8_t y);

#endif