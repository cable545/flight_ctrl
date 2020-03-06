/**
  ******************************************************************************
  * @file    uart.h
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @copy
  *
  * <h2><center>&copy; COPYRIGHT 2014 TKtronic </center></h2>
  */
#ifndef __UART_H_
#define __UART_H_

/* -----------------------------------------------------------------------------
 *                              enum's and macros                               
 * -----------------------------------------------------------------------------
 */



/* -----------------------------------------------------------------------------
 *                              typedef's                               
 * -----------------------------------------------------------------------------
 */




/* -----------------------------------------------------------------------------
 *                              function prototypes                               
 * -----------------------------------------------------------------------------
 */

/*------------------------------+
 | Defines                      |
 +------------------------------*/
//#define SER_RXMASK    0x3f    /* for a 64 byte ring buffer */
//#define SER_RXMASK    0x7f    /* for a 128 byte ring buffer */
//#define SER_RXMASK    0xff    /* for a 256 byte ring buffer */
//#define SER_RXMASK    0x1ff    /* for a 512 byte ring buffer */
//#define SER_RXMASK    0x1ff    /* for a 512 byte ring buffer */
#define SER_RXMASK    2047

/* XON/XOFF chars for SW-flow control */
#define UART_XON     0x11
#define UART_XOFF    0x13


#define UART_USE_ISR  1
//#define UART_USE_POLLING  1


/*------------------------------+
 | Structures & Unions          |
 +------------------------------*/
typedef enum
{
    UART_P_NONE = 0,
    UART_P_EVEN,
    UART_P_ODD,
    UART_P_MAX
}UartParity_t;

typedef enum
{
    UART_F_NONE = 0,
    UART_F_XON_XOFF,
    UART_F_RTS_CTS,
    UART_F_MAX
}UartFlow_t;


/*------------------------------+
 | Prototypes                   |
 +------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
	extern void UART_Rewind(void);
	extern void UART_Flush(void);
	extern uint8 UART_Init(uint8 *rxbuf, uint32 baud, uint8 dbits, uint8 start,
						   UartParity_t stop, UartFlow_t flowctrl);
	extern uint16 UART_RxAvailable(void);
	extern uint8 UART_Putch(uint8 c);
	extern uint8 UART_Getch(void);
	extern uint8 UART_TxAvailable (void);
	extern void UART_PutChar(uint8 c);
	extern void UART_PutStr(uint8 *s);

	#ifdef UART_USE_POLLING
		extern void UART_PollHandler(void);
	#else
		extern void UART_RxHandler(uint8 c);
		extern void UART_TxHandler(void);
		extern uint8 UART_TxBuffer (uint8 *txbuf, uint16 l);
		extern uint8 UART_TxDone(void);
	#endif

	extern void UART2_Rewind(void);
	extern void UART2_Flush(void);
	extern uint8 UART2_Init(uint8 *rxbuf, uint32 baud, uint8 dbits, uint8 start,
						   UartParity_t stop, UartFlow_t flowctrl);
	extern uint16 UART2_RxAvailable(void);
	extern uint8 UART2_Putch(uint8 c);
	extern uint8 UART2_Getch(void);
	extern uint8 UART2_TxAvailable (void);
	extern void UART2_PutChar(uint8 c);
	extern void UART2_PutStr(uint8 *s);

	#ifdef UART_USE_POLLING
		extern void UART2_PollHandler(void);
	#else
		extern void UART2_RxHandler(uint8 c);
		extern void UART2_TxHandler(void);
		extern uint8 UART2_TxBuffer (uint8 *txbuf, uint16 l);
		extern uint8 UART2_TxDone(void);
	#endif

	extern void UART4_Rewind(void);
	extern void UART4_Flush(void);
	extern uint8 UART4_Init(uint8 *rxbuf, uint32 baud, uint8 dbits, uint8 start,
						   UartParity_t stop, UartFlow_t flowctrl);
	extern uint16 UART4_RxAvailable(void);
	extern uint8 UART4_Putch(uint8 c);
	extern uint8 UART4_Getch(void);
	extern uint8 UART4_TxAvailable (void);
	extern void UART4_PutChar(uint8 c);
	extern void UART4_PutStr(uint8 *s);

	#ifdef UART_USE_POLLING
		extern void UART4_PollHandler(void);
	#else
		extern void UART4_RxHandler(uint8 c);
		extern void UART4_TxHandler(void);
		extern uint8 UART4_TxBuffer (uint8 *txbuf, uint16 l);
		extern uint8 UART4_TxDone(void);
	#endif
#ifdef __cplusplus
}
#endif

#endif /* __UART_H_ */
