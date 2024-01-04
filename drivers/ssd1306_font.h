#ifndef _SSD1306_FONT_
#define _SSD1306_FONT_

typedef struct{
    unsigned char    width;
    unsigned char    height;
    unsigned int     font[];
} SSD1306_font;

#endif