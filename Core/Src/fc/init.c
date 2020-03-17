/*
 * init.c
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#include <stdint.h>

#include "drivers/system.h"
#include "drivers/buzzer.h"

#include "fc/init.h"
#include "fc/tasks.h"

#include "hw/spi.h"

#include "io/status_leds.h"



uint8_t systemState = SYSTEM_STATE_INITIALISING;

void init(void)
{
	SYSTEM_Init();

	STATUS_LEDS_Init();
	BUZZER_Init();
	SPI3_Init();
	MPU_Init();

	//tasksInit();

	systemState |= SYSTEM_STATE_READY;
}
