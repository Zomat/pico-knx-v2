#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "app/knx.h"
#include "app/oled.h"
#include "ssi.h"
#include "cgi.h"
#include "app/webserver.h"
#include "app/event.h"
#include "app/eventHandler.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

DisplayAddress addresses[] = {
    {"DEVICE IP", server_ip},
    {"KNX SOURCE", knx_source_address},
    {"KNX TARGET", knx_target_address}
};

absolute_time_t last_event_time;
bool should_clear_display = false;

const uint btn_pin = 7;

int main() {
    stdio_init_all();
    cyw43_arch_init();

    gpio_init(btn_pin);
    gpio_set_dir(btn_pin, GPIO_IN);
    gpio_pull_up(btn_pin);

    sleep_ms(500);

    bool ap_mode;
    ap_mode = ! gpio_get(btn_pin);

    knx_init();
    oled_init();
    oled_show_splash_screen();
    webserver_init(ap_mode);

    SSD1306_ClearDisplay(&oled);

    oled_show_addresses(addresses, sizeof(addresses) / sizeof(DisplayAddress));

    // Infinite loop
    while(1) {
        Event e = event_dequeue();
        if (e.type != -1) {
            switch (e.type) {
                case KNX_SWITCH_EVENT:
                    handle_knx_switch_event(e.data);
                    should_clear_display = true;
                    last_event_time = get_absolute_time();
                    break;
                
                case KNX_SET_ADDR_EVENT:
                    handle_knx_set_address_event(e.data);
                    should_clear_display = true;
                    last_event_time = get_absolute_time();
                    break;

                case KNX_DIMMING_EVENT:
                    handle_knx_dimming_event(e.data);
                    should_clear_display = true;
                    last_event_time = get_absolute_time();
                    break;
            }
        }

        if (should_clear_display && absolute_time_diff_us(last_event_time, get_absolute_time()) >= 1000000) {
            SSD1306_ClearDisplay(&oled);
            oled_show_addresses(addresses, sizeof(addresses) / sizeof(DisplayAddress));
            should_clear_display = false;
        }
    };
}