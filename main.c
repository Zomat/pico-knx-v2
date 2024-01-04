#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "drivers/SSD1306_I2C.h"
#include "drivers/KnxTelegram.h"
#include "app/knx.h"

#define OLED_SDA 10
#define OLED_SCL 11

// WIFI Credentials
const char WIFI_SSID[] = "BITSwitch";
const char WIFI_PASSWORD[] = "leqz-1zez-nk0f";

char *server_ip;
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

void webserver_init()
{
    cyw43_arch_enable_sta_mode();

    // Connect to the WiFI network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
        SSD1306_WriteText(&oled, "CONNECTING...");
    }
    // Print a success message once connected
    printf("Connected! \n");
    
    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");
    server_ip = ip4addr_ntoa(netif_ip4_addr(netif_list));
    printf("Starting server at %s on port %s\n", server_ip, "80");

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");
}

typedef struct {
  char* name;
  char* address;
} DisplayAddress;

int main() {
    stdio_init_all();
    cyw43_arch_init();
    oled_init();
    webserver_init();    

    DisplayAddress addresses[] = {
        {"SERVER IP", server_ip},
        {"KNX SOURCE", knx_source_address},
        {"KNX TARGET", knx_target_address}
    };

    SSD1306_ClearDisplay(&oled);

    for (uint8_t i = 0; i < sizeof(addresses) / sizeof(DisplayAddress); i++) {
        SSD1306_WriteText(&oled, strcat(addresses[i].name, ": "));
        SSD1306_NextLine(&oled);
        SSD1306_WriteText(&oled, addresses[i].address);
        SSD1306_NextLine(&oled);
        SSD1306_NextLine(&oled);
    }
    
    // Infinite loop
    while(1);
}