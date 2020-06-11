/*
Copyright 2019 Layne Guan <layne001365@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hal.h"
#include "wait.h"

#include <print.h>

#define BTSPI   SPID1

static const SPIConfig spicfg = {
  .end_cb = NULL,
  .ssport = GPIOA,
  .sspad  = 4,
  .cr1    = SPI_CR1_BR_1 | SPI_CR1_BR_0,
  .cr2    = 0
};

void bt_init(void)
{
	// IRQ
	palSetPadMode(GPIOB, 13, PAL_MODE_OUTPUT_PUSHPULL);
	palClearPad(GPIOB, 13);

	// INT
	palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLDOWN);
	palClearPad(GPIOB, 14);

	spiAcquireBus(&BTSPI);
	spiStart(&SPID1, &spicfg);
	spiSelect(&SPID1);
}

void bt_send(uint8_t n, uint8_t *txbuf)
{
	uprintf(">>> CR1=%x CR2=%x SR=%x DR=%x CRCPR=%x RXCRCR=%x TXCRCR=%x I2SCFGR=%x I2SPR=%x.\r\n",
			BTSPI.spi->CR1, BTSPI.spi->CR2, BTSPI.spi->SR, BTSPI.spi->DR,
			BTSPI.spi->CRCPR, BTSPI.spi->RXCRCR, BTSPI.spi->TXCRCR, BTSPI.spi->I2SCFGR, BTSPI.spi->I2SPR);

	palSetPad(GPIOB, 13);

	spiStartSend(&BTSPI, n, txbuf);

	wait_ms(1);
	palClearPad(GPIOB, 13);
	
	uprintf(">>spi send:");
	for (int i = 0; i < n; ++i)
	{
		printf("0x%02x ", txbuf[i]);
	}
	printf("\r\n");
	uprintf("<<< CR1=%x CR2=%x SR=%x DR=%x CRCPR=%x RXCRCR=%x TXCRCR=%x I2SCFGR=%x I2SPR=%x.\r\n",
			BTSPI.spi->CR1, BTSPI.spi->CR2, BTSPI.spi->SR, BTSPI.spi->DR,
			BTSPI.spi->CRCPR, BTSPI.spi->RXCRCR, BTSPI.spi->TXCRCR, BTSPI.spi->I2SCFGR, BTSPI.spi->I2SPR);
}


void bt_recv(uint8_t n, uint8_t *rxbuf)
{
	spiStartReceive(&BTSPI, n, rxbuf);
}

void bt_route(void)
{
	uint8_t status = palReadPad(GPIOB, 14);

	if (status != 0 )
	{
		uprintf("Got SPI Master interupt!!! \n\r");
	}
}

int bt_cmd_1(void)
{
	static bool flag = false;
	uint8_t buf[3] = { 0xA6, 0x51, 0x89, };

	if (!flag)
	{
		bt_send(3, buf);
	}
	else
	{
		buf[2] = 0x81;
		bt_send(3, buf);
	}

	flag = !flag;

    return 0;
}

int bt_cmd_2(void)
{
	uint8_t buf[3] = { 0xA6, 0x51, 0x62, };

	bt_send(3, buf);

    return 0;
}
