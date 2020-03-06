/*
 * platform.h
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#if defined(STM32F407VGTx) || defined (STM32F415xx)

#include "stm32f4xx.h"

// Chip Unique ID on F405
#define U_ID_0 (*(uint32_t*)0x1fff7a10)
#define U_ID_1 (*(uint32_t*)0x1fff7a14)
#define U_ID_2 (*(uint32_t*)0x1fff7a18)

#ifndef STM32F4
#define STM32F4
#endif

#endif

#endif
