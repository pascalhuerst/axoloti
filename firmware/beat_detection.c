/**
 * Copyright (C) 2016, Pascal Huerst
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
#include "beat_detection.h"

// -> mcuconf.h : PA0 TIM2_CH1
static const ICUConfig icu_config = {
      ICU_INPUT_ACTIVE_HIGH,	// Measure on active edge
      1000000,			// 1 MHz timer clock
      pulse_width_callback,
      periode_width_callback,
      timer_overflow_callback,
      ICU_CHANNEL_1		// TIM2_CH1
    };

static ICUDriver icu_driver;
static uint32_t dt[2];
static mutex_t icu_driver_mutex;

static THD_WORKING_AREA(wa_icu_driver_thread, 128);
static THD_FUNCTION(icu_driver_thread, arg) {

	uint32_t time_diff;

	while (true) {
		chMtxLock(&icu_driver_mutex);
			time_diff = dt[0] - dt[1];
		chMtxUnlock(&icu_driver_mutex);
		// Do something
		chThdSleep(TIME_INFINITE);
	}
}

void init_beatdetection()
{
	palSetPadMode(GPIOA, 0, PAL_MODE_INPUT);
	palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(1));	// TIM2_CH1

	chMtxInit(&icu_driver_mutex);

	icuObjectInit(&icu_driver);
	icuEnable(&icu_driver);
	icuStart(&icu_driver, &icu_config);
}

void pulse_width_callback(ICUDriver *icu_driver)
{
	chMtxLock(&icu_driver_mutex);
	dt[1] = dt[0];
	dt[0] = icuGetWidth(icu_driver);
	chMtxUnlock(&icu_driver_mutex);

	chThdResume(icu_driver_thread);
}

void periode_width_callback(ICUDriver *driver)
{


}

void timer_overflow_callback(ICUDriver *driver)
{


}
