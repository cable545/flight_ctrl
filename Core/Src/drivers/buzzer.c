/*
 * buzzer.c
 *
 *  Created on: 25.02.2020
 *      Author: burde
 */

#include "platform.h"

#include "drivers/buzzer.h"
#include "drivers/io.h"
#include "drivers/system.h"

#define BUZZER_ON				(GPIO_HIGH(GPIOC,GPIO_PIN_0)); (GPIO_LOW(GPIOC,GPIO_PIN_1))
#define BUZZER_OFF			(GPIO_LOW(GPIOC,GPIO_PIN_0)); (GPIO_HIGH(GPIOC,GPIO_PIN_1))
#define BEEP_DELAY for(uint32_t i = 5000; i > 0; i--)

static void beep(void)
{
	for(int j = 500; j != 0; j--)
	{
		BUZZER_ON;
		BEEP_DELAY;
		BUZZER_OFF;
		BEEP_DELAY;
	}
}

void BUZZER_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	// buzzer pins for diff mode
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void BUZZER_Beep(uint32_t times)
{
	for(uint32_t i = times; i != 0; i--)
	{
		beep();
		delay(500);
	}
}
