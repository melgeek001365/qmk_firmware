#pragma once

#include <string.h>

void bt_uart_init(void);
void bt_uart_deinit(void);
bool bt_uart_send_cmd(void);
