#include "stm8s.h"
#include "mystm8sxxx.h"


/* Declaring Global Variables */
#define BT1_PORT	GPIOE
#define BT1_PIN		GPIO_PIN_0

#define BT2_PORT	GPIOD
#define BT2_PIN		GPIO_PIN_0

volatile uint8_t record_flag =0, stop_flag=0;

/* Declaring Function Prototype */
void uart_send_string(uint8_t * string,uint8_t length);
void delay(uint32_t time);

void main(void)
{

	/* Initialize clock using HSE */
	clk_config_16MHz_hse();

	/* Configure BT1 and BT2 as input with interrupt */
	GPIO_Init(BT1_PORT,BT1_PIN,GPIO_MODE_IN_PU_IT);
	GPIO_Init(BT2_PORT,BT2_PIN,GPIO_MODE_IN_PU_IT);

	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOE,EXTI_SENSITIVITY_FALL_ONLY);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD,EXTI_SENSITIVITY_FALL_ONLY);

	/*	Configure UART1 115200 - 8bit data - None Parity - One Stop bit
	 *	UART1 TX (PA5)
	 *	UART1 RX (PA4)
	 */
	UART1_Init(115200,
			UART1_WORDLENGTH_8D,
			UART1_STOPBITS_1,
			UART1_PARITY_NO,
			UART1_SYNCMODE_CLOCK_DISABLE,
			UART1_MODE_TX_ENABLE);

	/* Enable UART1 Module */
	UART1_Cmd(ENABLE);

	/* Global interrupt enable */
	enableInterrupts();

	while (1)
	{
		if(record_flag)
		{
			record_flag = 0;
			uart_send_string((uint8_t*)"\r3",2);
		}
		else if(stop_flag)
		{
			stop_flag =	0;
			uart_send_string((uint8_t*)"\r4",2);
		}
	}
}

/* User Function */
void uart_send_string(uint8_t * string,uint8_t length)
{
	for(uint8_t i=0;i<length;i++)
	{
		while(!UART1_GetFlagStatus(UART1_FLAG_TC));
		UART1_SendData8(*(string+i));
	}
}

void delay(uint32_t time)
{
	while(time--);
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval : None
 */
void assert_failed(u8* file, u32 line)
{ 
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
