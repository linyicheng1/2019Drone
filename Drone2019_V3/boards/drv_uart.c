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
  *          debug
  *          USART2
  *            PA3---RX
  *            PA2---TX
  */
#include "drv_uart.h"
#include "board.h"
#include "dbus.h"
#include "volt+.h"
#include "string.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

static HAL_StatusTypeDef debuger_hook(void);
static HAL_StatusTypeDef judgement_hook(void);
static HAL_StatusTypeDef remoteControl_hook(void);

uint8_t RC_receiveBuffer[36];
uint8_t Debug_receiveBuffer[9];
float Debug_receive[4];
/**
  * @brief  串口接收回调函数
  * @param  NONE 
  * @retval NONE
  */
uint64_t deltaT;
short temp_length = 1;
short temp_discode = 0;
short temp_begin = 30;
uint8_t temp_RC_receiveBuffer[18];
void UART3_receiveData_hook(void)
{
	static short length = 0; 
	
	static uint64_t lastTime;
	static short state = 0;
	__HAL_UART_CLEAR_IDLEFLAG(&huart3);
	deltaT = GetSysTimeUs() - lastTime;
	lastTime = GetSysTimeUs();
	/* clear idle it flag avoid idle interrupt all the time */

	/* clear DMA transfer complete flag */
	__HAL_DMA_DISABLE(huart3.hdmarx);

	/* handle dbus data dbus_buf from DMA */
	length = 36 - huart3.hdmarx->Instance->CNDTR;
	if(deltaT>3000)
	{
		if(length != 1&&state==0)
		{//如果不是1，则把这一帧丢弃
			if(temp_discode == 0)
			{
//				huart3.hdmarx->Instance->CNDTR = temp_begin;
				temp_discode = 1;
			}
			//报错
			temp_length = length;
			state = -1;
		}
		else 
		{
//			state = 0;
		}
	}
	if (length == 18)
	{
		huart3.hdmarx->Instance->CNDTR = 36;
		memcpy(&temp_RC_receiveBuffer[18-temp_length+1],RC_receiveBuffer,temp_length-1);
		Sbus_Decode(temp_RC_receiveBuffer);
		memcpy(temp_RC_receiveBuffer,&RC_receiveBuffer[temp_length-1],18-temp_length+1);
	}		
	/* restart dma transmission */		
	__HAL_DMA_ENABLE(huart3.hdmarx);
}
/**
  * @brief  串口接收回调函数
  * @param  NONE 
  * @retval NONE
  */
void UART2_receiveData_hook (void)
{
	USART2_Cmd();
}
/**
  * @brief  使能串口1
  * @param  none
  * @retval none
  */
void USART1_Cmd(void)
{
	
}
/**
  * @brief  使能串口2
  * @param  none
  * @retval none
  */
void USART2_Cmd(void)
{
//	while(HAL_UART_Receive_DMA(&huart2,Debug_receiveBuffer,9)!=HAL_OK);
}
void Debug_discode(void)
{
	Volt_discode(Debug_receiveBuffer,Debug_receive);
}
/**
  * @brief  使能串口3
  * @param  none
  * @retval none
  */
void USART3_Cmd(void)
{
	while(HAL_UART_Receive_DMA(&huart3,RC_receiveBuffer,30)!=HAL_OK);
	
}
/**
  * @brief  调试串口发送波形函数
  * @param  data: 发送数据指针
  * @param  size: Size data 4
  * @retval HAL status
  */
uint16_t sendLength;
uint8_t sendData[20];
HAL_StatusTypeDef debuger_Send_Wave(float * data,int length)
{
	
	sendLength = Volt_encode(sendData,data,length);
	HAL_UART_Transmit_DMA(&huart2,sendData,sendLength);
}

/**
  * @brief  调试串口接受中断回调
  * @param  
  * @param  
  * @retval HAL status
  */
static HAL_StatusTypeDef debuger_hook(void)
{
	
}
/**
  * @brief  裁判系统接收函数
  * @param  
  * @param  
  * @retval HAL status
  */
static HAL_StatusTypeDef judgement_hook(void)
{
	
}

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}


