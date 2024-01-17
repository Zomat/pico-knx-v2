#include "app/eventHandler.h"

bool handle_knx_switch_event(void *data)
{
  SSD1306_ClearDisplay(&oled);
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "KNX SWITCH %s", ((int) data) == 1 ? "ON" : "OFF");
  SSD1306_WriteImage(&oled, &image_switch, 40, 0);
  SSD1306_NextLine(&oled);
  SSD1306_NextLine(&oled);
  SSD1306_WriteText(&oled, buffer);

  return true;
}

bool handle_knx_set_address_event(void *data)
{
  SSD1306_ClearDisplay(&oled);
  char buffer[20];
  char addr_buffer[11];
  memset(addr_buffer, 0, sizeof(addr_buffer));
  snprintf(addr_buffer, sizeof(addr_buffer), "%s", (char*) data);
  snprintf(buffer, sizeof(buffer), "KNX ADDRESS");
  SSD1306_WriteImage(&oled, &image_address, 40, 0);
  SSD1306_NextLine(&oled);
  SSD1306_font *temp_font = oled.fontPtr;
  SSD1306_SetFont(&oled, "5x8");
  SSD1306_WriteText(&oled, buffer);
  SSD1306_NextLine(&oled);
  SSD1306_WriteText(&oled, addr_buffer);
  oled.fontPtr = temp_font;

  return true;
}

bool handle_knx_dimming_event(void *data)
{
  SSD1306_ClearDisplay(&oled);
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "KNX DIMMING %d", ((int) data));
  SSD1306_WriteImage(&oled, &image_dimming, 40, 0);
  SSD1306_NextLine(&oled);
  SSD1306_NextLine(&oled);
  SSD1306_WriteText(&oled, buffer);

  return true;
}