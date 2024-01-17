#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "lwipopts.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "app/dhcpserver.h"
#include <string.h>
#include "wifipass.h"

extern const char wifi_ssid[];
extern const char wifi_password[];

extern char const ap_ssid[];
extern char const ap_password[];

extern char server_ip[16];

void webserver_init(bool ap_mode);

#endif //_WEBSERVER_H_