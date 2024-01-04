#ifndef _SSD1306_IMAGE_
#define _SSD1306_IMAGE_

typedef struct{
    unsigned int    width;
    unsigned int    height;
    unsigned char   image[];
} SSD1306_image;

#endif