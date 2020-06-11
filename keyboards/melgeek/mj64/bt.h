
void bt_init(void);

void bt_send(uint8_t n, uint8_t *txbuf);
void bt_recv(uint8_t n, uint8_t *rxbuf);

void bt_route(void);


int bt_cmd_1(void);
int bt_cmd_2(void);
