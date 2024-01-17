#include "webserver.h"

char const wifi_ssid[] = WIFI_SSID;
char const wifi_password[] = WIFI_PASS;

char const ap_ssid[] = AP_SSID;
char const ap_password[] = AP_PASS;

char server_ip[16];

dhcp_server_t dhcp_server;
ip4_addr_t mask, ip;

void webserver_init(bool ap_mode)
{
    if (ap_mode) {
        cyw43_arch_enable_ap_mode(ap_ssid, ap_password, CYW43_AUTH_WPA2_AES_PSK);
        printf("AP mode enabled\n");
        printf("Starting server at %s on port %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), "80");
    } else {
        cyw43_arch_enable_sta_mode();

        // Connect to the WiFI network - loop until connected
        while(cyw43_arch_wifi_connect_timeout_ms(wifi_ssid, wifi_password, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
            printf("Attempting to connect...\n");
        }
    }
    
    // Print a success message once connected
    printf("Connected! \n");
    
    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");
    strcpy(server_ip, ip4addr_ntoa(netif_ip4_addr(netif_list)));
    printf("Starting server at %s on port %s\n", server_ip, "80");

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");

    if (ap_mode) {
        IP4_ADDR(ip_2_ip4(&mask), 255, 255, 255, 0);
        IP4_ADDR(ip_2_ip4(&ip), 192, 168, 4, 1);
        dhcp_server_init(&dhcp_server, &ip, &mask);

        printf("DHCP server initialised\n");
    }
}