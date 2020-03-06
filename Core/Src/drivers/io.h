/*
 * io.h
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#include "platform.h"

#ifndef DRIVERS_IO_H_
#define DRIVERS_IO_H_

#define GPIO_HIGH(a,b)         a->ODR |= b
#define GPIO_LOW(a,b)        a->ODR &= ~(b)
#define GPIO_TOGGLE(a,b)     a->ODR ^= b

#endif
