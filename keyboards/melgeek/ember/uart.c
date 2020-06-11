#include "ch.h"
#include "hal.h"

#include "print.h"

static virtual_timer_t vt1, vt2;

static void restart(void *p) {

  (void)p;

  chSysLockFromISR();
  uartStartSendI(&UARTD2, 14, "Hello World!\r\n");
  chSysUnlockFromISR();
}

static void ledoff(void *p) {

  (void)p;
}

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {
  (void)uartp;
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {

  (void)uartp;

  chSysLockFromISR();
  chVTResetI(&vt1);
  chVTDoSetI(&vt1, TIME_MS2I(5000), restart, NULL);
  chSysUnlockFromISR();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {

  (void)uartp;
  (void)c;
  uprintf("%02x ");

  /* Flashing the LED each time a character is received.*/
  chSysLockFromISR();
  chVTResetI(&vt2);
  chVTDoSetI(&vt2, TIME_MS2I(200), ledoff, NULL);
  chSysUnlockFromISR();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg = {
  txend1,
  txend2,
  rxend,
  rxchar,
  rxerr,
  NULL,
  0,
  38400,
  0,
  USART_CR2_LINEN,
  0
};

void bt_uart_init(void)
{
	uartStart(&UARTD2, &uart_cfg);
}

void bt_uart_deinit(void)
{

}

bool bt_uart_send_cmd(void)
{
	return true;
}

