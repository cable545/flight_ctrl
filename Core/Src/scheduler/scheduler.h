/*
 * scheduler.h
 *
 *  Created on: Feb 20, 2020
 *      Author: burde
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "common/time.h"

#define SCHEDULER_DELAY_LIMIT 10

#define TASK_PERIOD_HZ(hz) (1000000 / (hz))
#define TASK_PERIOD_MS(ms) ((ms) * 1000)
#define TASK_PERIOD_US(us) (us)

#define GYRO_TASK_GUARD_INTERVAL_US 10  // Don't run any other tasks if gyro task will be run soon

typedef enum
{
	TASK_PRIORITY_REALTIME = -1, // Task will be run outside the scheduler logic
	TASK_PRIORITY_IDLE = 0,      // Disables dynamic scheduling, task is executed only if no other task is active this cycle
	TASK_PRIORITY_LOW = 1,
	TASK_PRIORITY_MEDIUM = 3,
	TASK_PRIORITY_MEDIUM_HIGH = 4,
	TASK_PRIORITY_HIGH = 5,
	TASK_PRIORITY_MAX = 255
} cfTaskPriority_e;

typedef struct
{
	const char * taskName;
	const char * subTaskName;
	bool         isEnabled;
	int8_t       staticPriority;
	timeDelta_t  desiredPeriod;
	timeDelta_t  latestDeltaTime;
	timeUs_t     maxExecutionTime;
	timeUs_t     totalExecutionTime;
	timeUs_t     averageExecutionTime;
	timeUs_t     averageDeltaTime;
	float        movingAverageCycleTime;
} cfTaskInfo_t;

typedef enum
{
	/* Actual tasks */
	TASK_SYSTEM = 0,
	TASK_MAIN,
	TASK_STATUS_LED,
	/* Count of real tasks */
	TASK_COUNT,

	/* Service task IDs */
	TASK_NONE = TASK_COUNT,
	TASK_SELF
} cfTaskId_e;

typedef struct
{
	// Configuration
#if defined(USE_TASK_STATISTICS)
	const char * taskName;
	const char * subTaskName;
#endif
	bool (*checkFunc)(timeUs_t currentTimeUs, timeDelta_t currentDeltaTimeUs);
	void (*taskFunc)(timeUs_t currentTimeUs);
	timeDelta_t desiredPeriod;      // target period of execution
	const int8_t staticPriority;    // dynamicPriority grows in steps of this size

	// Scheduling
	uint16_t dynamicPriority;       // measurement of how old task was last executed, used to avoid task starvation
	uint16_t taskAgeCycles;
	timeDelta_t taskLatestDeltaTime;
	timeUs_t lastExecutedAt;        // last time of invocation
	timeUs_t lastSignaledAt;        // time of invocation event for event-driven tasks
	timeUs_t lastDesiredAt;         // time of last desired execution

#if defined(USE_TASK_STATISTICS)
	// Statistics
	float    movingAverageCycleTime;
	timeUs_t movingSumExecutionTime;  // moving sum over 32 samples
	timeUs_t movingSumDeltaTime;  // moving sum over 32 samples
	timeUs_t maxExecutionTime;
	timeUs_t totalExecutionTime;    // total time consumed by task since boot
#endif
} cfTask_t;

extern cfTask_t cfTasks[TASK_COUNT];

void taskSystemLoad(timeUs_t currentTimeUs);
void setTaskEnabled(cfTaskId_e taskId, bool enabled);
void schedulerInit(void);
void scheduler(void);

#endif
