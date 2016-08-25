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

#ifndef __SERIAL_DMX__
#define __SERIAL_DMX__

#include <stdint.h>

void serial_dmx_init(void);
void serial_dmx_set(uint16_t address, uint8_t value);
uint8_t serial_dmx_data[513];

#endif
