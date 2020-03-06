/*
 * init.c
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#include <stdint.h>

#include "drivers/system.h"

#include "fc/init.h"
#include "fc/tasks.h"

#include "io/status_leds.h"

#include "drivers/buzzer.h"

uint8_t systemState = SYSTEM_STATE_INITIALISING;

void init(void)
{
	SYSTEM_Init();

	STATUS_LEDS_Init();
	BUZZER_Init();
//	SPI3_Init();

	tasksInit();

	systemState |= SYSTEM_STATE_READY;
}
