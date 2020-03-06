/*
 * maths.h
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#ifndef SCHEDULER_MATHS_H_
#define SCHEDULER_MATHS_H_

#define MAX(a,b) \
  __extension__ ({ __typeof__ (a) _a = (a); \
  __typeof__ (b) _b = (b); \
  _a > _b ? _a : _b; })

#endif
