/*
 * nvic.h
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#ifndef DRIVERS_NVIC_H_
#define DRIVERS_NVIC_H_

#define ENABLE_INT          __set_PRIMASK(0);
#define DISABLE_INT         __set_PRIMASK(1);

#define NVIC_PRIO_MAX                      NVIC_BUILD_PRIORITY(0, 1)

#define NVIC_BUILD_PRIORITY(base,sub) (((((base)<<(4-(7-(NVIC_PRIORITY_GROUPING>>8))))|((sub)&(0x0f>>(7-(NVIC_PRIORITY_GROUPING>>8)))))<<4)&0xf0)

#endif
