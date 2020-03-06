/*
 * init.h
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#ifndef FC_INIT_H_
#define FC_INIT_H_

typedef enum {
	SYSTEM_STATE_INITIALISING   = 0,
	SYSTEM_STATE_CONFIG_LOADED  = (1 << 0),
	SYSTEM_STATE_SENSORS_READY  = (1 << 1),
	SYSTEM_STATE_MOTORS_READY   = (1 << 2),
	SYSTEM_STATE_TRANSPONDER_ENABLED = (1 << 3),
	SYSTEM_STATE_READY          = (1 << 7)
} systemState_e;

void init(void);

#endif
