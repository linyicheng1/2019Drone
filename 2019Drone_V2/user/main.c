#include "main.h"
#include "modules.h"
DroneState drone_state;
int main(void)
{
	droneConfig();
	droneInit();
	while (1)
	{
	}
}
RCC_ClocksTypeDef rcc;
void droneConfig(void)
{
	RCC_GetClocksFreq(&rcc);
	SystemInit();
	while(SysTick_Config(72000));		
	CAN_Config();
	TIM_Config();
	USART_Config();
}
void droneInit(void)
{
	modules_init();
}
void SysTick_Handler(void)
{

	drone_state.time.systick_ms++;
	if(drone_state.time.systick_ms>=1000)
	{
		drone_state.time.systick_s ++;
		drone_state.time.systick_ms = 0;
		if(drone_state.time.systick_s>=60)
		{
			drone_state.time.systick_min ++;
			drone_state.time.systick_s = 0;
		}
	}
	modules_task();
}
uint32_t get_systick_ms(void)
{
	return drone_state.time.systick_ms;
}
uint32_t get_systick_s(void)
{
	return drone_state.time.systick_s;
}
uint32_t get_systick_min(void)
{
	return drone_state.time.systick_min;
}