#include "SSD1306_I2C.h"

void SSD1306_WriteCmd(SSD1306 *dev, uint8_t cmd)
{
    uint8_t x[2] = {SSD1306_CMD_REG, cmd};
    i2c_write_blocking(dev->i2cHandler, SSD1306_I2C_ADDR, x, 2, false);
}

void SSD1306_WriteData(SSD1306 *dev, uint8_t data)
{
    uint8_t x[2] = {SSD1306_DATA_REG, data};
    i2c_write_blocking(dev->i2cHandler, SSD1306_I2C_ADDR, x, 2, false);
    
    // Change actual column after writing data
    if (dev->column < SSD1306_WIDTH - 1) {
        dev->column++;
    } else {
        dev->column = 0;
    }
}

void SSD1306_SetCursor(SSD1306 *dev, uint8_t x, uint8_t y)
{
    SSD1306_WriteCmd(dev, 0x00 + (x & 0x0F)); // set column lower addres
    SSD1306_WriteCmd(dev, 0x10 + ((x >> 4) & 0x0F)); // set column higher addres
    SSD1306_WriteCmd(dev, 0xB0 + y); // set page addres

    dev->column   = x;
    dev->page     = y;
}

void SSD1306_NextLine(SSD1306 *dev)
{
    if (dev->page == SSD1306_PAGES-1) {
        return;
    }

    SSD1306_SetCursor(dev, 0, dev->page+(dev->fontPtr->height/SSD1306_PAGE_HEIGHT));
}

void SSD1306_ClearDisplay(SSD1306 *dev)
{
    for (uint8_t page = 0; page < 8; page++) {
        SSD1306_ClearPage(dev, page);
    }

    SSD1306_SetCursor(dev, 0, 0);
}

void SSD1306_ClearPage(SSD1306 *dev, uint8_t page)
{
    uint8_t actualPage = dev->page;

    SSD1306_SetCursor(dev, 0, page);
    for (uint8_t column = 0; column < SSD1306_WIDTH; column++) {
        SSD1306_WriteData(dev, 0x00);
    }

    SSD1306_SetCursor(dev, 0, actualPage);
}

void SSD1306_ClearSpace(SSD1306* dev, uint8_t x0, uint8_t x1, uint8_t page0, uint8_t page1)
{
    uint8_t swapTemp;
    uint8_t actualPage = dev->page;
    uint8_t actualColumn = dev->column;

    if (x0 > x1) {
        swapTemp = x0;
        x0 = x1;
        x1 = swapTemp;
    }

    if (page0 > page1) {
        swapTemp = page0;
        page0 = page1;
        page1 = swapTemp;
    }

    for (int page = page0; page <= page1; page++) {
        SSD1306_SetCursor(dev, x0, page);
        for (int column = x0; column <= x1; column++) {
            SSD1306_WriteData(dev, 0x00);
        }
    }

    SSD1306_SetCursor(dev, actualColumn, actualPage);
}

// Get index of given character start item in font array
uint SSD1306_GetFontIndex(char ch, uint fontWidth)
{
    return (uint)((ch-0x20)*fontWidth);
}

void SSD1306_WriteChar(SSD1306 *dev, char ch)
{
    uint8_t font_width = dev->fontPtr->width;
    uint char_index = SSD1306_GetFontIndex(ch, font_width);

    // Break line
    if (dev->column + font_width >= SSD1306_WIDTH 
        && dev->page < SSD1306_PAGES-1) {
        SSD1306_SetCursor(dev, 0, dev->page+(dev->fontPtr->height/SSD1306_PAGE_HEIGHT));
        
        // If space, dont print it
        if (ch == 0x20) {
            return;
        }
    } 

    // Print column by column
    for (uint8_t i = 0; i < font_width; i++) {
        if (*((dev->fontPtr->font)+char_index+i) > (1 << SSD1306_PAGE_HEIGHT+1)) {
            SSD1306_WriteData(dev, (*((dev->fontPtr->font)+char_index+i)) & 0xFF);
            SSD1306_SetCursor(dev, dev->column-1, dev->page+1);
            SSD1306_WriteData(dev, (*((dev->fontPtr->font)+char_index+i) >> 8) & 0xFF);
            SSD1306_SetCursor(dev, dev->column, dev->page-1);

            continue;
        }

        SSD1306_WriteData(dev, *((dev->fontPtr->font)+char_index+i));
    }
}

void SSD1306_WriteText(SSD1306 *dev, char *str)
{
    uint8_t actualPage = dev->page;

    for (uint i = 0; i < strlen(str); i++) {
        SSD1306_WriteChar(dev, str[i]);
    }

    SSD1306_SetCursor(dev, dev->column, actualPage);
}

void SSD1306_WriteImage(SSD1306 *dev, SSD1306_image *image, uint8_t x, uint8_t y)
{
    uint8_t img_parts = image->height/SSD1306_PAGE_HEIGHT;
    for (uint i = 0; i < img_parts; i++) {
        SSD1306_SetCursor(dev, x, i+y);
        for (uint j = 0; j < image->width; j++) {
            SSD1306_WriteData(dev, image->image[(i*image->width)+j]);
        }
    }
}

void SSD1306_SetFont(SSD1306 *dev, char *size)
{
    if (size == "3x5") {
        dev->fontPtr = &font3x5;
        return;
    }

    if (size == "5x8") {
        dev->fontPtr = &font5x8;
        return;
    }

    if (size == "8x8") {
        dev->fontPtr = &font8x8;
        return;
    }

    if (size == "8x16") {
        dev->fontPtr = &font8x16;
        return;
    }

    if (size == "16x16") {
        dev->fontPtr = &font16x16;
        return;
    }
}

void SSD1306_WritePixel(SSD1306 *dev, uint8_t x, uint8_t y) {
    uint8_t actualPage = dev->page;
    uint8_t actualColumn = dev->column;

    SSD1306_SetCursor(dev, x, (y / SSD1306_PAGE_HEIGHT));
    SSD1306_WriteData(dev, (1 << (y % SSD1306_PAGE_HEIGHT)));

    SSD1306_SetCursor(dev, actualColumn, actualPage);
}

void SSD1306_init(SSD1306 *dev, i2c_inst_t *i2cHandler)
{
    dev->i2cHandler = i2cHandler;

    SSD1306_WriteCmd(dev, SSD1306_SET_DISPLAY_ON);
    SSD1306_WriteCmd(dev, SSD1306_SET_DISPLAY_NORMAL);
    SSD1306_WriteCmd(dev, SSD1306_SET_MEMORY_MODE);
    SSD1306_WriteCmd(dev, SSD1306_PAGE_ADDRESSING_MODE);
    SSD1306_WriteCmd(dev, 0x8D); //charge pump
    SSD1306_WriteCmd(dev, 0x14);
    SSD1306_ClearDisplay(dev);

    SSD1306_SetFont(dev, "8x8");
}