/*
 * system.c
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#include "platform.h"

#include "drivers/nvic.h"
#include "drivers/system.h"

#include "io/status_leds.h"

#define DWT_LAR_UNLOCK_VALUE 0xC5ACCE55

// cycles per microsecond
static uint32_t usTicks = 0;
// current uptime for 1kHz systick timer. will rollover after 49 days. hopefully we won't care.
static volatile uint32_t sysTickUptime = 0;
static volatile uint32_t sysTickValStamp = 0;

static uint32_t cpuClockFrequency = 0;

static void cycleCounterInit(void)
{
#if defined(USE_HAL_DRIVER)
	cpuClockFrequency = HAL_RCC_GetSysClockFreq();
#else
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	cpuClockFrequency = clocks.SYSCLK_Frequency;
#endif
	usTicks = cpuClockFrequency / 1000000;

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void SYSTEM_Init(void)
{
	SystemInit();
	//SysTick_Config(SystemCoreClock / 1000); // SysTick 1ms ticks
	//cycleCounterInit();
}

// SysTick

static volatile int sysTickPending = 0;

void SysTick_Handler(void)
{
	DISABLE_INT;
	sysTickUptime++;
	sysTickValStamp = SysTick->VAL;
	sysTickPending = 0;
	(void)(SysTick->CTRL);
	ENABLE_INT;
#ifdef USE_HAL_DRIVER
    // used by the HAL for some timekeeping and timeouts, should always be 1ms
    HAL_IncTick();
#endif
}

// Return system uptime in microseconds (rollover in 70minutes)
uint32_t microsISR(void)
{
	register uint32_t ms, pending, cycle_cnt;

	DISABLE_INT;
	cycle_cnt = SysTick->VAL;

	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
	{
		// Update pending.
		// Record it for multiple calls within the same rollover period
		// (Will be cleared when serviced).
		// Note that multiple rollovers are not considered.

		sysTickPending = 1;

		// Read VAL again to ensure the value is read after the rollover.

		cycle_cnt = SysTick->VAL;
	}

	ms = sysTickUptime;
	pending = sysTickPending;

	ENABLE_INT;

	return ((ms + pending) * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

uint32_t micros(void)
{
	register uint32_t ms, cycle_cnt;

	// Call microsISR() in interrupt and elevated (non-zero) BASEPRI context

	if((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) || (__get_BASEPRI()))
		return microsISR();

	do
	{
		ms = sysTickUptime;
		cycle_cnt = SysTick->VAL;
	} while (ms != sysTickUptime || cycle_cnt > sysTickValStamp);

	return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

void delayMicroseconds(uint32_t us)
{
	uint32_t now = micros();
	while(micros() - now < us);
}

void delay(uint32_t ms)
{
	while(ms--)
		delayMicroseconds(1000);
}


