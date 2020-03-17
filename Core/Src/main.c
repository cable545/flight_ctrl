
//#include <stdio.h>
//#include <math.h>

#include <stdbool.h>

#include "platform.h"

#include "drivers/buzzer.h"
#include "drivers/mpu.h"
#include "drivers/system.h"

#include "fc/init.h"

#include "scheduler/scheduler.h"

static float accBuffer[3], gyroBuffer[3];

void run(void);

//int main(void)
//{
//	UART2_Init((uint8 *)&Rxbuf[0], 115200, 8, 1, UART_P_NONE, UART_F_NONE);
//
//	LINKV2_Init();
//
//	MAIN_5msecsExpired();
//	MAIN_OneSectimerExpired();
//
//
//	while(1)
//	{
//		MAIN_ComCheck((Commv2Msg_t *) &RxFrame);
//	}
//}

int main(void)
{
  init();

  //BUZZER_Beep(3);

  run();

	return 0;
}

void run(void)
{
	uint8_t reg = 0;


	while(true)
	{

		//delay(10);
		//reg = MPU_readWhoAmIReg();
		MPU_ReadScaledAccData(accBuffer);
		MPU_ReadScaledGyroData(gyroBuffer);
		//scheduler();
		//processLoopback();
#ifdef SIMULATOR_BUILD
		//delayMicroseconds_real(50); // max rate 20kHz
#endif
	}
}


