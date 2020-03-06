/*
 * time.h
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#ifndef COMMON_TIME_H_
#define COMMON_TIME_H_

#include <stdint.h>

typedef int32_t timeDelta_t;
typedef uint32_t timeMs_t ;
typedef uint32_t timeUs_t;

static inline timeDelta_t cmpTimeUs(timeUs_t a, timeUs_t b) { return (timeDelta_t)(a - b); }

#endif
