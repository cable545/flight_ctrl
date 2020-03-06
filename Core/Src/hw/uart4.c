/**
  ******************************************************************************
  * @file    uart4.c
  * @author  T. Kuckenburg
  * @version 
  * @date    
  * @brief   uart driver for stm32f407 USART4 on PA0/PA1 or PC10/PC11
  ******************************************************************************
  *
  * <h2><center>&copy; 2014 TKtronic </center></h2>
  */
 
/* Includes ------------------------------------------------------------------*/
#include "platform.h"

#include "uart.h"

#ifdef UART_USE_ISR
    #define ENABLE_UART_RX_ISR     USART_ITConfig(UART4, USART_IT_RXNE, ENABLE)
    #define DISABLE_UART_RX_ISR    USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
    #define ENABLE_UART_TX_ISR     USART_ITConfig(UART4, USART_IT_TC, ENABLE)
    #define DISABLE_UART_TX_ISR    USART_ITConfig(UART4, USART_IT_TC, DISABLE);
#endif


/* Variables ------------------------------------------------------------------*/

/* ptr to circular rx-buffer */
static volatile uint8_t *Rxbuffer;
/* counting semaphore with number of unread chars in rx-buffer */
static volatile uint16_t Rxsema;
/* rx-buffer write ptr */
static volatile uint16_t Rxidx;
/* rx-buffer read ptr */
static volatile uint16_t Rxreadidx;

static volatile uint8_t *Txbuffer;
/* counting semaphore with number of chars left to send in tx-buffer */
static volatile uint16_t Txsema;

/* Functions ------------------------------------------------------------------*/

/**
  * @brief
  * @par
  * @retval
  * @par Required preconditions:
  */
uint8_t UART4_Init(uint8_t *rxbuf, uint32_t baud, uint8_t dbits, uint8_t stop,
                UartParity_t parity, UartFlow_t flowctrl)
{
    uint8_t err = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    Rxbuffer = rxbuf;
    Rxsema = 0;
    Rxidx = 0;
    Rxreadidx = 0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // UART alternative-functions pin connect
//    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
//    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    /* Configure USART4 Tx (PC.10) and Rx (PC.11) */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* Configure USART4 Tx (PA0) and Rx (PA1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_OverSampling8Cmd(UART4, ENABLE);
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    if (UART_P_EVEN == parity)
    {
        USART_InitStructure.USART_Parity = USART_Parity_Even;      
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    }
    else if(UART_P_ODD == parity)  
    {
        USART_InitStructure.USART_Parity = USART_Parity_Odd;      
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    }
    else
    {
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    }
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART4, &USART_InitStructure);
    USART_Cmd(UART4, ENABLE);
    /* dummy read from UART Rx-register */
    err = USART_ReceiveData(UART4);
#ifdef UART_USE_ISR
    DISABLE_UART_TX_ISR;
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ENABLE_UART_RX_ISR;
    USART_ClearITPendingBit(UART4, USART_IT_TC);
#endif

return(err);
}

/**
  * @brief  This function handles UART4 global interrupt requests.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
    uint8_t c;

    if((UART4->SR & 0x0020) > 0x0000)
    {/* Read byte from the receive data register */
        c = UART4->DR;
        UART4_RxHandler(c);
    }
    if((UART4->SR & 0x0040) > 0x0000)
    {
        USART_ClearITPendingBit(UART4, USART_IT_TC);
        UART4_TxHandler();
    }
}

/**
  * @brief Rx Isr Handler user function
  * @par uint8_t char from RxD register
  * @retval
  * @par Required preconditions:
  * @caller USART ISR
  */
void UART4_RxHandler(uint8_t c)
{
    Rxbuffer[Rxidx++] = c; /* get char */
    Rxidx &= SER_RXMASK; /* wrap around ring buffer idx */
    Rxsema++;    /* inc semaphore */
} 

#ifdef UART_USE_ISR
/**
  * @brief Tx Isr Handler user function
  * @par None
  * @retval    None
  * @par Required preconditions:
  * @caller USARTn ISR
  */
void UART4_TxHandler(void)
{
    if (0 < Txsema)
    {
        UART4->DR = *Txbuffer;
        Txbuffer++;
        Txsema--;
    }
    else
    { // last char sent
        DISABLE_UART_TX_ISR;
    }
} 

uint8_t UART4_TxDone(void)
{
    if ((Txsema == 0) && (UART4->SR & 0x0080) > 0x0000)
        return(TRUE);
    else
        return(FALSE);      
}

/**
  * @brief Initiate transmission of a buffer with given length
  * @par ptr to buffer, length
  * @retval TRUE if uart TX-Reg. available, otherwise FALSE
  * @par Required preconditions:
  */
uint8_t UART4_TxBuffer (uint8_t *txbuf, uint16_t l)
{
    uint8_t err = OK;
    
    UART4->DR = *txbuf;
    Txsema = l-1;
    Txbuffer = txbuf+1;
    while(!((UART4->SR & 0x0080) > 0x0000));
    ENABLE_UART_TX_ISR;

return(err);
}
#endif

/**
  * @brief flushes the RX-buffer by setting all indexes to 0
  * @par
  * @retval
  * @par Required preconditions:
  */
void UART4_Flush(void)
{
  #ifdef UART_USE_ISR
    DISABLE_UART_RX_ISR;
  #endif
    Rxsema = 0;
    Rxidx = 0;
    Rxreadidx = 0;
  #ifdef UART_USE_ISR
    ENABLE_UART_RX_ISR;
  #endif
}


/**
  * @brief  rewind the read-ptr to start of buffer, no chars are lost
  * @par
  * @retval
  * @par Required preconditions:
  */
void UART4_Rewind(void)
{
#ifdef UART_USE_ISR
  DISABLE_UART_RX_ISR;
#endif
    Rxsema = Rxidx;
    Rxreadidx = 0;
  #ifdef UART_USE_ISR
    ENABLE_UART_RX_ISR;
  #endif
}

/**
  * @brief returns number of chars are available in Rx-buffer
  * @par
  * @retval
  * @par Required preconditions:
  */
uint16_t UART4_RxAvailable(void)
{
    return Rxsema;
}

/**
  * @brief gets a char from the Rx-Buffer
  * @par none
  * @retval rx'ed char
  * @par Required preconditions:
  */
uint8_t UART4_Getch(void)
{
    uint8_t retval;

#ifdef UART_USE_ISR
  DISABLE_UART_RX_ISR;
#endif
    retval = Rxbuffer[Rxreadidx];

    if (Rxsema > 0)    /* new char available in RxBuffer */
    {
        Rxreadidx++;
        Rxreadidx &= SER_RXMASK;  /* ring buf ptr wrap around */
        Rxsema--;
    }
  #ifdef UART_USE_ISR
    ENABLE_UART_RX_ISR;
  #endif
    return retval;
}

/**
  * @brief place a char to Tx
  * @par character to be transmitted
  * @retval same char on success
  * @par Required preconditions: waits for an empty TX-buffer
  */
uint8_t UART4_Putch (uint8_t c)
{
    while (!UART_TxAvailable());
    UART4->DR = c;
    return (c);
}

/**
  * @brief test if uart is available for Tx of new char
  * @par none
  * @retval TRUE if uart TX-DR empty flag set, otherwise FALSE
  * @par Required preconditions:
  */
uint8_t UART4_TxAvailable (void)
{
    if ((UART4->SR & 0x0080) > 0)
        return TRUE;
    else
    {
        return FALSE;
    }
}

/**
  * @brief Custom function to Tx char to serial comm device 
  * @par character for TX
  * @retval none
  * @par Required preconditions:
  */
void UART4_PutChar(uint8_t c) /* send char via serial port */
{
       UART4_Putch(c);
}

/**
  * @brief Custom function to Tx a string to serial comm device 
  * @par NUL character terminated string
  * @retval none
  * @par Required preconditions: uses polling mode for Tx
  */
void UART4_PutStr(uint8_t *s)
{
    while (*s)
        UART_PutChar(*s++);
}

