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
#include "chprintf.h"


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

static Mutex icu_driver_mutex;
static EventSource icu_trigger_event;

static WORKING_AREA(wa_icu_driver_thread, 128);

__attribute__((noreturn))
static msg_t icu_driver_thread(void *arg) {

	uint32_t time_diff;
	EventListener icu_trigger_listener;
	chEvtRegisterMask(&icu_trigger_event, &icu_trigger_listener, EVENT_MASK(1));

	while (true) {
		chMtxLock(&icu_driver_mutex);
			time_diff = dt[0] - dt[1];
		chMtxUnlock();
		// Do something

		chEvtWaitOne(EVENT_MASK(1));
		chprintf((BaseSequentialStream * )&SD2,"%s\n", __func__);
	}
}

struct ICUConfig *beatdetection_config() { return &icu_config; }
struct ICUDriver *beatdetection_driver() { return &icu_driver; }

void init_beatdetection()
{
	palSetPadMode(GPIOA, 0, PAL_MODE_INPUT);
	palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(1));	// TIM2_CH1

	chMtxInit(&icu_driver_mutex);

	chEvtInit(&icu_trigger_event);

	icuObjectInit(&icu_driver);
	icuEnable(&icu_driver);
	chprintf((BaseSequentialStream * )&SD2,"%s\n", __func__);
	chprintf((BaseSequentialStream * )&SD2,"%s\n", __func__);
}

void pulse_width_callback(ICUDriver *icu_driver)
{
	chMtxLock(&icu_driver_mutex);
	dt[1] = dt[0];
	dt[0] = icuGetWidth(icu_driver);
	chMtxUnlock();

	chprintf((BaseSequentialStream * )&SD2,"%s\n", __func__);
	chEvtBroadcastI(&icu_trigger_event);
}

void periode_width_callback(ICUDriver *driver)
{

	chprintf((BaseSequentialStream * )&SD2,"%s\n", __func__);

}

void timer_overflow_callback(ICUDriver *driver)
{

	chprintf((BaseSequentialStream * )&SD2,"%s\n", __func__);

}
