#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "app/knx.h"
#include "app/event.h"


// CGI handler which is run when a request for /led.cgi is detected
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Check if an request for LED has been made (/led.cgi?led=x)
    if (strcmp(pcParam[0] , "led") == 0){
        // Look at the argument to check if LED is to be turned on (x=1) or off (x=0)
        if(strcmp(pcValue[0], "0") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        else if(strcmp(pcValue[0], "1") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    }
    
    // Indicate to lwIP that the response has been handled
    return "/index.shtml";
}

const char * cgi_knx_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    if (strcmp(pcParam[0], "switch") == 0) {
        if(strcmp(pcValue[0], "0") == 0) {
            if (knx_switch(false)) {
                Event e = {KNX_SWITCH_EVENT, (void*) 0};
                event_enqueue(e);
            }
        }
        else if(strcmp(pcValue[0], "1") == 0) {
           if (knx_switch(true)) {
                Event e = {KNX_SWITCH_EVENT, (void*) 1};
                event_enqueue(e);
            }
        }
    }

    if (strcmp(pcParam[0], "dimming") == 0) {
        int value = atoi(pcValue[0]);
        printf("DIMMING: %d", value);
        if (value >= 0 && value <= 255) {
            if (knx_dimming(value)) {
                Event e = {KNX_DIMMING_EVENT, (void*) value};
                event_enqueue(e);
            }
        }
    }
    
    return "/index.shtml";
}

const char * cgi_knx_source_address_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    if (strcmp(pcParam[0] , "addr") == 0) {
        if(knx_set_source_address(pcValue[0])) {
            Event e = {KNX_SET_ADDR_EVENT, (void*) pcValue[0]};
            event_enqueue(e);
        }
    }
    
    return "/index.shtml";
}

const char * cgi_knx_target_address_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    if (strcmp(pcParam[0] , "addr") == 0) {
        if(knx_set_target_address(pcValue[0])) {
            Event e = {KNX_SET_ADDR_EVENT, (void*) pcValue[0]};
            event_enqueue(e);
        }
    }
    
    return "/index.shtml";
}

// tCGI Struct
// Fill this with all of the CGI requests and their respective handlers
static const tCGI cgi_handlers[] = {
    {
        "/led.cgi", cgi_led_handler
    },
    {
        "/knx.cgi", cgi_knx_handler
    },
    {
        "/edit_knx_source_address.cgi", cgi_knx_source_address_handler
    },
    {
        "/edit_knx_target_address.cgi", cgi_knx_target_address_handler
    },
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));
}