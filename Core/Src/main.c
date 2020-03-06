
//#include <stdio.h>
//#include <math.h>

#include <stdbool.h>

#include "platform.h"

#include "fc/init.h"

#include "scheduler/scheduler.h"

#include "drivers/buzzer.h"

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

  BUZZER_Beep(3);

  run();

	return 0;
}

void run(void)
{
	while(true)
	{
		scheduler();
		//processLoopback();
#ifdef SIMULATOR_BUILD
		//delayMicroseconds_real(50); // max rate 20kHz
#endif
	}
}


