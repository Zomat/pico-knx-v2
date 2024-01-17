#ifndef _OLED_H_
#define _OLED_H_

#include "drivers/SSD1306_I2C.h"
#include "drivers/ssd1306_image.h"

// Include images
#include "oledImages/image_my.h"
#include "oledImages/image_switch.h"
#include "oledImages/image_dimming.h"
#include "oledImages/image_address.h"

#define OLED_SDA 10
#define OLED_SCL 11

extern SSD1306 oled;

typedef struct {
  char* name;
  char* address;
} DisplayAddress;

void oled_init();

void oled_show_splash_screen();
void oled_show_addresses(DisplayAddress addresses[], size_t size);

#endif //_OLED_H_