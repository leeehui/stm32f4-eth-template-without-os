
  
#include "debug_usart.h"


///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		HAL_UART_Transmit(&DEBUG_USART, (uint8_t *) &ch, 1, 0xFFFF);
		return (ch);
}


void UARTInterruptHandler( USART_TypeDef *USARTx )
{
  if(LL_USART_IsActiveFlag_RXNE(USARTx) && LL_USART_IsEnabledIT_RXNE(USARTx))
  {
    /* RXNE flag will be cleared by reading of RDR register (done in call) */
    /* Call function in charge of handling Character reception */
    uint8_t data = LL_USART_ReceiveData8(USARTx);	
    queue_put(data);
  } 
}



/*********************************************END OF FILE**********************/
