/*
 * task.c
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#include <stdbool.h>
#include <stddef.h>

#include "common/utils.h"

#include "fc/tasks.h"

#include "io/status_leds.h"

#include "scheduler/scheduler.h"

static void taskMain(timeUs_t currentTimeUs)
{
	UNUSED(currentTimeUs);
}

void tasksInit(void)
{
	schedulerInit();

	setTaskEnabled(TASK_MAIN, true);
	setTaskEnabled(TASK_STATUS_LED, true);
}

#if defined(USE_TASK_STATISTICS)
#define DEFINE_TASK(taskNameParam, subTaskNameParam, checkFuncParam, taskFuncParam, desiredPeriodParam, staticPriorityParam) {  \
    .taskName = taskNameParam, \
    .subTaskName = subTaskNameParam, \
    .checkFunc = checkFuncParam, \
    .taskFunc = taskFuncParam, \
    .desiredPeriod = desiredPeriodParam, \
    .staticPriority = staticPriorityParam \
}
#else
#define DEFINE_TASK(taskNameParam, subTaskNameParam, checkFuncParam, taskFuncParam, desiredPeriodParam, staticPriorityParam) {  \
    .checkFunc = checkFuncParam, \
    .taskFunc = taskFuncParam, \
    .desiredPeriod = desiredPeriodParam, \
    .staticPriority = staticPriorityParam \
}
#endif

cfTask_t cfTasks[TASK_COUNT] = {
	[TASK_SYSTEM] = DEFINE_TASK("SYSTEM", "LOAD", NULL, taskSystemLoad, TASK_PERIOD_HZ(10), TASK_PRIORITY_MEDIUM_HIGH),
	[TASK_MAIN] = DEFINE_TASK("SYSTEM", "UPDATE", NULL, taskMain, TASK_PERIOD_HZ(1000), TASK_PRIORITY_MEDIUM_HIGH),
	[TASK_STATUS_LED] = DEFINE_TASK("STATUS_LED", NULL, NULL, STATUS_LEDS_ToggleLed1, TASK_PERIOD_HZ(1), TASK_PRIORITY_LOW),
};
