#include "knx.h"
#include "drivers/KnxTelegram.h"

char knx_source_address[] = "0.0.1";
char knx_target_address[] = "0.0.2";

static bool sendKnxTelegram(const char telegram[], int messageSize) {
    uint8_t sendbuf[2];
    for (int i = 0; i < messageSize; i++) {
        if (i == (messageSize - 1)) {
            sendbuf[0] = TPUART_DATA_END;
        }  else {
            sendbuf[0] = TPUART_DATA_START_CONTINUE;
        }

        sendbuf[0] |= i;
        sendbuf[1] = telegram[i];

        uart_putc_raw(UART_ID, sendbuf[0]);
        uart_putc_raw(UART_ID, sendbuf[1]);
    }

    return true;
}