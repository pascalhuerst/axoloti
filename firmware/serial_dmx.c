/**
 * Copyright (C) 2016 Pascal Huerst
 *
 * This file is part of Axoloti.
 *
 * Axoloti is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Axoloti is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Axoloti. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ch.h"
#include "hal.h"
#include "axoloti_board.h"
#include "midi.h"
#include "patch.h"
#include "serial_dmx.h"

static VirtualTimer vt1;

static void restart(void *p) {

  (void)p;

  chSysLockFromIsr();
//  palSetPadMode(GPIOA, 2, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  uartStartSend(&UARTD2, 513, serial_dmx_data);
  chSysUnlockFromIsr();
}

static void txend1(UARTDriver *uartp) {
  (void)uartp;
}

static void txend2(UARTDriver *uartp) {

  //Do the end shit with gpio here. Reload DMX buffer and send again in 1/40Hz

  (void)uartp;
  chSysLockFromIsr();

  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(0));
  palWritePad(GPIOA, 2, 0);
  if (chVTIsArmedI(&vt1))
    chVTResetI(&vt1);
  chVTSetI(&vt1, MS2ST(25), restart, NULL);
  chSysUnlockFromIsr();
}

static const UARTConfig uart_cfg = {
  txend1,
  txend2,
  0,
  0,
  0,
  250000,
  0,
  USART_CR2_STOP_1, /* 2 Stopbits  */
  0
};


void serial_dmx_set(uint16_t address, uint8_t value)
{
	//Do the Mutex shit
  serial_dmx_data[address+1] = value;
}

void serial_dmx_init(void)
{
  int i = 0;

  for (i=0; i<513; i++) {
	  serial_dmx_data[i] = 0;
  }

  uartStart(&UARTD2, &uart_cfg);
  palSetPadMode(GPIOA, 2, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));

  uartStartSend(&UARTD2, 513, serial_dmx_data);
}
