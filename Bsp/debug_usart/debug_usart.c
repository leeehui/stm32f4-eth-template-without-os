
  
#include "debug_usart.h"



///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		HAL_UART_Transmit(&DEBUG_USART, (uint8_t *) &ch, 1, 0xFFFF);
		return (ch);
}

/*********************************************END OF FILE**********************/
