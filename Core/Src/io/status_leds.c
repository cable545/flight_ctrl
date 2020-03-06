/*
 * status_led.c
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#include "platform.h"

#include "drivers/io.h"

#include "io/status_leds.h"

#define LED1_TOGGLE	(GPIO_TOGGLE(GPIOB,GPIO_PIN_9))
#define LED2_TOGGLE	(GPIO_TOGGLE(GPIOB,GPIO_PIN_8))

static uint8_t ledState = 0;

void STATUS_LEDS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	// green LED on board
	GPIO_HIGH(GPIOB, GPIO_PIN_9);
	GPIO_InitStructure.Pin =  GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	// red LED on board
	GPIO_HIGH(GPIOB, GPIO_PIN_8);
	GPIO_InitStructure.Pin =  GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void STATUS_LEDS_ToggleLed1(void)
{
	LED1_TOGGLE;
	LED2_TOGGLE;

	ledState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
}
