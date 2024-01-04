#include "knx.h"
#include "drivers/KnxTelegram.h"

char knx_source_address[] = "0.0.1";
char knx_target_address[] = "0.0.2";

static bool send_telegram(const char telegram[], int messageSize) {
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

bool knx_set_source_address(char addr[])
{
  strcpy(knx_source_address, addr);
  return true;
}

bool knx_set_target_address(char addr[])
{
  strcpy(knx_target_address, addr);
  return true;
}

bool knx_switch(bool state)
{
  uint8_t telegram[9];
  uint16_t data;
  uint8_t checksum;
  uint8_t controlByte = knx_create_control_field(false, "auto");
  uint16_t sourceAddress = knx_create_source_address_field_from_string(knx_source_address);
  uint16_t targetAddress = knx_create_target_group_address_field_from_string(knx_target_address);
  uint8_t byte5 = 0x00;
  knx_set_target_address_type(&byte5, true);
  knx_set_routing_counter(&byte5, 6);  
  knx_set_data_length(&byte5, 1);  

  /* === Fill telegram === */
  telegram[0] = controlByte;
  telegram[1] = (sourceAddress >> 8) & 0x00FF;
  telegram[2] = (sourceAddress & 0x00FF);
  telegram[3] = (targetAddress >> 8) & 0x00FF;
  telegram[4] = (targetAddress) & 0x00FF;
  telegram[5] = byte5;
  data = knx_create_data_switch_field(KNX_CMD_VALUE_WRITE, state);
  telegram[6] = (data >> 8) & 0x00FF;
  telegram[7] = data & 0x00FF;
  telegram[8] = knx_calculate_checksum(telegram, sizeof(telegram)/sizeof(uint8_t));

  return send_telegram(telegram, 9);
}