/**
  * @file    drv_uart.c
  * @author  lyc
  * @brief   uart operations
  * @pinAssignment
  *          judgment---USART1
  *            PA9---TX
  *            PA10---RX
  *          remote---USART3
  *            PB11---RX
  *          debug---UART5
  *            PD2---RX
  *            PC12---TX
  *          other 
  *          USART2
  *            PA3---RX
  *            PA2---TX
  *          UART4
  *            PC11---RX
  *            PC10---TX  
  */
#include "usart.h"
#include "dma.h"
#include "drv_uart.h"
#include "stm32f1xx_hal_dma.h"
#include "device.h"
struct uart_manage_obj uart1_manage;
struct uart_manage_obj uart2_manage;
struct uart_manage_obj uart3_manage;
struct uart_manage_obj uart4_manage;
struct uart_manage_obj uart5_manage;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_uart5_tx;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
/**
  * @brief  串口发送函数
  * @retval None
  */
void UART_sendData(uint8_t *tx_bufferPrt,uint8_t uart,uint16_t size)
{
	UART_HandleTypeDef *uartPrt;
	switch(uart)
	{
		case judgment:uartPrt = &huart1;break;
		case remote:uartPrt = &huart3;break;	
		case debug:uartPrt = &huart5;break;
        case other_usart2:uartPrt = &huart2;break;
		case other_usart4:uartPrt = &huart4;break;		
	}
	HAL_UART_Transmit_DMA(uartPrt,tx_bufferPrt,size);
}
/**
  * @brief  串口接受中断调用函数
  * @retval None
  */
void UART_receiveData_hook(uint8_t uart )
{
	switch(uart)
	{
		case judgment:UART1_receiveData_hook();break;
		case remote:UART3_receiveData_hook();break;	
		case debug:UART5_receiveData_hook();break;
        case other_usart2:UART2_receiveData_hook();break;
		case other_usart4:UART4_receiveData_hook();break;
        default :break;		
	}
}
/**
  * @brief  串口接受中断调用函数,for usart1
  * @retval None
  */
void UART1_receiveData_hook(void)
{

}
/**
  * @brief  串口接受中断调用函数,for usart2
  * @retval None
  */
void UART2_receiveData_hook(void)
{
	
}
/**
  * @brief  串口接受中断调用函数,for usart3
  * @retval None
  */
extern uint8_t receiveBegin;
void UART3_receiveData_hook(void)
{
  if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))
  {
    /* clear idle it flag avoid idle interrupt all the time */
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);

    /* clear DMA transfer complete flag */
    __HAL_DMA_DISABLE(huart3.hdmarx);

    /* handle dbus data dbus_buf from DMA */
    if ((30 - huart3.hdmarx->Instance->CNDTR) == 18)
    {
		receiveBegin = 1;
		RemoteReceive();
		huart3.hdmarx->Instance->CNDTR = 30;
    }
    /* restart dma transmission */
	
    __HAL_DMA_ENABLE(huart3.hdmarx);
  }
}
/**
  * @brief  串口接受中断调用函数,for usart4
  * @retval None
  */
void UART4_receiveData_hook(void)
{
	
}
/**
  * @brief  串口接受中断调用函数,for usart5
  * @retval None
  */
void UART5_receiveData_hook(void)
{
	
}
